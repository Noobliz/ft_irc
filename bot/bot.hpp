#include <list>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <ctime> 
#include <csignal> 
#include <fcntl.h>

class Bot
{
	public:
		void	sendMsg(const std::string &msg)const;

		//void	init(int port, const std::string &nick);
		void	connectToServ();
		void	run();
		std::string 	getRandomMessage() const;
		void	sendBack(std::string concatstr);
		Bot(int port, const std::string &pass);

	private:
		Bot();


		int						_sockFd;
		struct sockaddr_in		_servAddr;
		uint16_t				_port;
		std::string				_password;
		std::string				_nickname;
};