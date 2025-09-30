#include <Server.hpp>

Server::Server(uint16_t const & port, std::string & password) : _port(port), _password(password)
{
	_commandMap["NICK"] = &Server::nick;
	_commandMap["PASS"] = &Server::pass;
	_commandMap["USER"] = &Server::user;
	_commandMap["JOIN"] = &Server::join;
	_commandMap["PRIVMSG"] = &Server::privmsg;
	_commandMap["KICK"] = &Server::kick;
	_commandMap["INVITE"] = &Server::invite;
	_commandMap["TOPIC"] = &Server::topic;
	_commandMap["MODE"] = &Server::mode;
	_commandMap["WHO"] = &Server::who;
	_commandMap["PART"] = &Server::part;
	_commandMap["QUIT"] = &Server::quit;
	_commandMap["bot"] = &Server::bot;
}


void Server::sendMsg(const std::string &msg, int fd)const
{
    std::string full_msg = msg + "\r\n";
    if (send(fd, full_msg.c_str(), full_msg.length(), 0) == -1)
        throw std::invalid_argument("Error: send fail");
}
Server::~Server()
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
	}
	close(_sockfd);
	close(_epollfd);
}


static std::string getPrefix(std::stringstream *sstream, bool *hasPrefix)
{
	std::string prefix;

	*hasPrefix = true;
	*sstream >> prefix;
	if (prefix == ":")
		throw std::invalid_argument("Error: prefix cant be null");
	return prefix.substr(1);
}


void	Server::chooseCmd(t_commandArgs & cArgs)
{
	std::string	cmd;
	*cArgs.sstream >> cmd;

	std::map<std::string, void (Server::*)(t_commandArgs&)>::iterator it = _commandMap.find(cmd);
	if (it != _commandMap.end())
	{
		(this->*(it->second))(cArgs);
	}
	else
	{
		//! attention feedback au client
		//! @time=2025-09-24T07:18:11.459Z :coven.IRC4Fun.net 421 rick1234 TOPIVC :Unknown command
		throw std::invalid_argument("Error: unknown command");
	}
}

int	Server::repartitor(Client & client, std::string str)
{
	std::cout << "string reçue dans repartitor : " << str << std::endl;

	t_commandArgs	cArgs;

	if (!str.empty())
	{
		bool hasPrefix = false;
		std::string prefix = "";
		try
		{
			std::stringstream sstream(str);
			if (str[0] == ':')
			{
				prefix = getPrefix(&sstream, &hasPrefix);
				std::cout << "prefix : " << prefix << std::endl;
			}
			cArgs.client = &client;
			cArgs.hasPrefix = hasPrefix;
			cArgs.prefix = prefix;
			cArgs.sstream = &sstream;
			chooseCmd(cArgs);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	return 0;
}

bool	catchSig = false;

void	sigHandler(int code)
{
	(void)code;
	catchSig = true;
}

void	Server::init(void)
{
	int opt = 1;

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
		throw std::invalid_argument("Error: socket fail");

	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::invalid_argument("Error: fcntl fail");

	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::invalid_argument("Error: setsockopt fail");

	if (bind(_sockfd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw std::invalid_argument("Error: bind fail");

	if (listen(_sockfd, MAX_CLIENT) == -1)
		throw std::invalid_argument("Error: listen fail");

	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
		throw std::invalid_argument("Error: epoll_create1 fail");
}

void	Server::run(void)
{
	socklen_t	addrlen = sizeof(_addr);
	std::string	concatstr = "";

	_ev.data.fd = _sockfd;
	_ev.events = EPOLLIN;

	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &_ev) == -1)
		throw std::invalid_argument("Error: epoll_ctl fail");

	signal(SIGINT, &sigHandler);
	signal(SIGQUIT, &sigHandler);
	while (1)
	{
		int	n = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
		if (n == -1)
		{
			if (errno == EINTR)
				break;
			else
				throw std::invalid_argument("Error: epoll_wait fail");
		}

		for (int i = 0; i < n; i++)
		{
			if (_events[i].data.fd == _sockfd)
			{
				int			tmpfd;

				tmpfd = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);
				if (tmpfd == -1)
					throw std::invalid_argument("Error: accept fail");
				//client.setFD(tmpfd);

				std::cout << "j'ai ecoute un client" << std::endl;

				_ev.events = EPOLLIN;
				//_ev.data.fd = client.getFD();
				_ev.data.fd = tmpfd;

				Client client(tmpfd);
				_clients[tmpfd] = client;

				// if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, client.getFD(), &_ev) == -1)
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, tmpfd, &_ev) == -1)
					throw std::invalid_argument("Error: epoll_ctl fail");

			}
			else
			{
				if (_events[i].events & EPOLLIN)
				{
					char buffer[2] = "";
					size_t r = recv(_events[i].data.fd, buffer, 1, 0);
					if (r == 0)
					{
						std::cout << "client fermax" << std::endl;

						doQuit(_clients[_events[i].data.fd], "Leaving.");
					}
					else if (r > 0)
					{
						//std::cout << "je recois un message" << std::endl;
						if (buffer[0] != '\n')
						{
							concatstr += buffer;
							//std::cout << concatstr << std::endl;
						}
						else
						{
							concatstr += "\n";
							repartitor(_clients[_events[i].data.fd], concatstr);
							concatstr = "";
						}

					}
					else
						throw std::invalid_argument("Error: recv fail");
				}

				if (_events[i].events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
				{
					std::cout << "the client number " << _events[i].data.fd << " has been disconnected." << std::endl;
				}
			}
		}
	}
}
