#pragma once

#include <includes.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdexcept>
#include <Client.hpp>
#include <Channel.hpp>
#include <map>

# define MAX_CLIENT 1024
# define MAX_EVENTS 256

class Server
{

	public :

		Server(uint16_t const & port);
		~Server();

		void	init(void);
		void	run(void);

	private :

		Server();

		uint16_t			_port;
		int					_sockfd;
		int					_epollfd;
		struct sockaddr_in	_addr;
		struct epoll_event	_events[MAX_EVENTS];
		struct epoll_event	_ev;

		std::map<std::string, Client>	_clients;
		std::map<std::string, Channel>	_channels;

};
