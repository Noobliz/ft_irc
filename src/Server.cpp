#include <Server.hpp>

Server::Server(uint16_t const & port, std::string & password) : _port(port), _password(password) {}

Server::~Server()
{
	close(_sockfd);
	close(_epollfd);
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

	_ev.data.fd = _sockfd;
	_ev.events = EPOLLIN;

	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &_ev) == -1)
		throw std::invalid_argument("Error: epoll_ctl fail");

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

				_waitingClients.push_back(Client(tmpfd));

				// if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, client.getFD(), &_ev) == -1)
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, tmpfd, &_ev) == -1)
					throw std::invalid_argument("Error: epoll_ctl fail");

			}
			else
			{
				if (_events[i].events & EPOLLIN)
				{
					//?detecter la commande. "JOIN #chan1" "PRIVMSG #chan1 :bonjour a tou.te.s"



					// char *buffer2 = (char *)calloc(10, 1);
					// size_t r = recv(_events[i].data.fd, buffer2, 10, 0);
					// if (r == 0)
					// {
					// 	std::cout << "client fermax" << std::endl;
					// 	close(_events[i].data.fd);
					// }
					// else if (r > 0)
					// {
					// 	if (buffer2[0] != 0)
					// 		std::cout << "buffer 2 = " << buffer2 << std::endl;
					// 	free(buffer2);
					// }
					// else
					// 	throw std::invalid_argument("Error: recv fail");

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
