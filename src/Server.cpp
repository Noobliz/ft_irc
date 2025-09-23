#include <Server.hpp>
#include <msgMacros.hpp>
#include <protocolMsg.hpp>

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
}

Server::~Server()
{
	close(_sockfd);
	close(_epollfd);
}


void	Server::doJoin(std::map<std::string, std::string> chanPwPair, bool resetUserChans, t_commandArgs cArgs)
{
	(void)resetUserChans;
	std::string	feedback;

	for (std::map<std::string, std::string>::iterator it = chanPwPair.begin(); it != chanPwPair.end(); ++it)
	{
		std::map<std::string, Channel>::iterator cit = _channels.find((*it).first);
		if (cit != _channels.end())
		{
			if (_channels[(*it).first].checkPassword((*it).second))
			{
				//? attention, addClient ne rajoutera pas de client s'il est pas invité et que le server est en inviteMode
				_channels[(*it).first].addClient(*cArgs.client);
				cArgs.client->addChan((*it).first, _channels[(*it).first]);
				feedback = JOIN(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, (*it).first);
				feedback += RPL_NAMREPLY(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, (*it).first);
				feedback += RPL_ENDOFNAMES(cArgs.client->getNickname(), (*it).first);
				feedback += RPL_NOTOPIC(cArgs.client->getNickname(), (*it).first);
				send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0);
				feedback = "";
			}
			else
			{
				//! err bad pass
			}
		}
		else
		{
			Channel newChan(*cArgs.client, (*it).first, (*it).second);
			_channels[(*it).first] = newChan;
			_channels[(*it).first].addClient(*cArgs.client);
			_channels[(*it).first].addOperator(*cArgs.client);
			cArgs.client->addChan((*it).first, _channels[(*it).first]);
			feedback = JOIN(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, (*it).first);
			feedback += RPL_NAMREPLY(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, (*it).first);
			feedback += RPL_ENDOFNAMES(cArgs.client->getNickname(), (*it).first);
			feedback += RPL_NOTOPIC(cArgs.client->getNickname(), (*it).first);
			send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0);
			feedback = "";
		}
	}
	/* accusé de réception ? */
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
		throw std::invalid_argument("Error: unknown command");
	}

	// (void)prefix;
	// if(cmd == "PASS")
	// {
	// 	if (hasPrefix)
	// 		throw std::invalid_argument("Error: PASS cant have a prefix.");
	// 	pass(sstream);
	// }
	// else if(cmd == "NICK")
	// 	nick(sstream, hasPrefix, prefix);
	// else if(cmd == "USER")
	// 	user(sstream, hasPrefix, prefix);
	// else if(cmd == "JOIN")
	// 	join(sstream, hasPrefix, prefix);
	// else if(cmd == "PRIVMSG")
	// 	privmsg(client, sstream, hasPrefix, prefix);
	// else if(cmd == "KICK")
	// 	kick(sstream, hasPrefix, prefix);
	// else if(cmd == "INVITE")
	// 	invite(sstream, hasPrefix, prefix);
	// else if(cmd == "TOPIC")
	// 	topic(sstream, hasPrefix, prefix);
	// else if(cmd == "MODE")
	// 		;//
	// else

	// return ;
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

	// signal(SIGINT, &sigHandler);
	// signal(SIGQUIT, &sigHandler);
	while (1)
	{
		int	n = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
		if (n == -1)
			throw std::invalid_argument("Error: epoll_wait fail");

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
					//?detecter la commande. "JOIN #chan1" "PRIVMSG #chan1 :bonjour a tou.te.s"

					// un buffer pour recevoir des trucs
					// passer ce qu'on a recu (sans doute faudra t il reconstruire le buffer a la maniere d'un gnl (tkt))
					// utiliser la fonction timRepartitor() avec le buffer reconstruit
					// puis dans timRepartitor, mettre nos fonctions a la suite du parsingTim()


                    //Ancienne version
					// char buffer[1024] = "";
					// size_t r = recv(_events[i].data.fd, buffer, 1024, 0);
					// if (r == 0)
					// {
					// 	std::cout << "client fermax" << std::endl;
					// 	close(_events[i].data.fd);
					// }
					// else if (r > 0)
					// {
					// 	std::cout << "je recois un message" << std::endl;
					// 	if (buffer[0] != 0)
					// 	{
					// 		concatstr = buffer;
					// 		std::cout << concatstr.size() << std::endl;
					// 		repartitor(_clients[_events[i].data.fd], concatstr);
					// 		concatstr = "";
					// 	}

					// }
					// else
					// 	throw std::invalid_argument("Error: recv fail");

                    char buffer[2] = "";
					size_t r = recv(_events[i].data.fd, buffer, 1, 0);
					if (r == 0)
					{
						std::cout << "client fermax" << std::endl;
						close(_events[i].data.fd);
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

					// size_t s = send(_events[i].data.fd, (void *)"j'ai bien recu ton message merci\n\0", 35, 0);

					// if (s == 0)
					// {
					// 	std::cout << "propre" << std::endl;
					// }
					// else if (s > 0)
					// {
					// 	std::cout << "retour ok" << std::endl;
					// }
					// else
					// {
					// 	//ici on peut modifier les events pour unlock EPOLLOUT et donc attendre que le buffer soit OK apparemment
					// }

				}

				if (_events[i].events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
				{
					std::cout << "the client number " << _events[i].data.fd << " has been disconnected." << std::endl;
				}
			}
		}
	}
}
