#pragma once

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdexcept>
#include <map>
#include <vector>
#include <stack>
#include <list>
#include <signal.h>

#include <cerrno>
#include <cstdlib>

#include <Client.hpp>
#include <Channel.hpp>
#include <protocolMsg.hpp>

# define MAX_CLIENT 1024
# define MAX_EVENTS 256

typedef struct s_commandArgs
{
	Client				*client;
	std::string			prefix;
	bool				hasPrefix;
	std::stringstream	*sstream;
}	t_commandArgs;

class Server
{

	public :

		Server(uint16_t const & port, std::string & password);
		~Server();

		void	init(void);
		void	run(void);

		int		repartitor(Client & client, std::string str);
		void	chooseCmd(t_commandArgs &cArgs);
		//Client	findClient(int fd) const;
		int		findClient(std::string nickname);

		void	doPrivateMsg(Client & client, std::vector<std::string> nick, std::string msg);
		void	doJoin(std::map<std::string, std::string> chanPwPair, bool resetUserChans, t_commandArgs cArgs);
		void	doTopic(t_commandArgs & cArgs, std::string & chan, std::string & topic);
		void	doInvite(Client & client, std::string const & target, std::string const &channel);
		void	doWho(std::string & chan, t_commandArgs & cArgs);
		void	doKick(Client & client, std::vector<std::string> const & target, std::string const &channel, std::string const &msg);
		void	doPart(Client & client, std::vector<std::string> chans, std::string msg);
		void	doQuit(Client & client, std::string msg);

	private :

		Server();

		void	user(t_commandArgs & cArgs);
		void	topic(t_commandArgs & cArgs);
		void	privmsg(t_commandArgs & cArgs);
		void	pass(t_commandArgs & cArgs);
		void	nick(t_commandArgs & cArgs);
		void	kick(t_commandArgs & cArgs);
		void	join(t_commandArgs & cArgs);
		void	invite(t_commandArgs & cArgs);
		void	mode(t_commandArgs & cArgs);
		void	who(t_commandArgs & cArgs);
		void	part(t_commandArgs & cArgs);
		void	quit(t_commandArgs & cArgs);

		uint16_t			_port;
		std::string			_password;
		int					_sockfd;
		int					_epollfd;
		struct sockaddr_in	_addr;
		struct epoll_event	_events[MAX_EVENTS];
		struct epoll_event	_ev;

		std::map<int, Client>			_clients;
		std::map<std::string, Channel>	_channels;
		std::map<std::string, void (Server::*)(t_commandArgs&)>	_commandMap;

};
