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
#include <vector>
#include <stack>
#include <list>

# define MAX_CLIENT 1024
# define MAX_EVENTS 256

class Server
{

	public :

		Server(uint16_t const & port, std::string & password);
		~Server();

		void	init(void);
		void	run(void);

		int		repartitor(Client & client, std::string str);
		int		findClient(std::string nickname);

		void	privateMsg(Client client, std::vector<std::string> nick, std::string msg);
	private :

		Server();

		uint16_t			_port;
		std::string			_password;
		int					_sockfd;
		int					_epollfd;
		struct sockaddr_in	_addr;
		struct epoll_event	_events[MAX_EVENTS];
		struct epoll_event	_ev;

		std::map<int, Client>			_clients;
		std::map<std::string, Channel>	_channels;

};
