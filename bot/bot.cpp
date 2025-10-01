#include "bot.hpp"

Bot::Bot(int port, const std::string &pass): _port(port), _password(pass), _nickname("beautyBot")
{
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_port = htons(_port);
	_servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr convert string in binary
	_sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd == -1)
		throw std::invalid_argument("Error: socket fail");
	srand(time(NULL));
}

Bot::~Bot(){}

bool	catchSig = false;

void	sigHandler(int code)
{
	(void)code;
	catchSig = true;
}

void Bot::sendMsg(const std::string &msg)const
{
	std::string full_msg = msg + "\r\n";
	if (send(_sockFd, full_msg.c_str(), full_msg.length(), 0) == -1)
	{
		close(_sockFd);
		throw std::invalid_argument("Error: send fail");
	}
}

void	Bot::connectToServ()
{
	socklen_t	addrlen = sizeof(_servAddr);

	if (connect(_sockFd, (struct sockaddr *)&_servAddr, addrlen) == -1)
	{
		close(_sockFd);
		throw std::invalid_argument("Error: connect fail");
	}

	sendMsg("NICK " + _nickname);
	sendMsg("USER " + _nickname + " 0 * :beautyBot");
	sendMsg("PASS " + _password);
}

std::string Bot::getRandomMessage() const
{
	const std::string messages[] = {
		"HELLO FROM BOT",
		"Did you know that polar bears can break their penis?",
		"Humans are so complicated",
		"Don't forget to do the dishes",
		"*Evil laugh*",
		"The meaning of life is 42",
		"I'm a bot, not a miracle worker",
		"Have you tried turning it off and on again?",
		"I'm definitely not a bot",
		"Error 404: response not found",
		"According to my calculations... you're wrong",
		"I could tell you, but then I'd have to debug you",
		"My other responses are in beta testing",
		"No.",
		"Did you know you were sooo pretty ?",
		"I am bot a not."
	};

	int	nbMsg = sizeof(messages) / sizeof(messages[0]);
	int	randomIndex = rand() % nbMsg;

	return messages[randomIndex];
}


void	Bot::sendBack(std::string concatstr)
{
	std::string			target;
	std::string			words;
	std::string			activated;
	std::stringstream	sstream(concatstr);
	int					count = 0;

	while (sstream >> words)
	{
		if (count == 0)
			activated = words;
		if (count == 1)
			target = words;
		count++;
	}
	if (activated != "activation")
		return;
	std::string msg = getRandomMessage();

	sendMsg("PRIVMSG "+ target + " :" + msg);
}

void	Bot::run()
{
	std::string	concatstr = "";
	bool		connected = true;

	if (fcntl(_sockFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::invalid_argument("Error: fcntl fail");

	signal(SIGINT, &sigHandler);
	signal(SIGQUIT, &sigHandler);
	while (connected && catchSig == false)
	{
		char buffer[2] = "";
		size_t r = recv(_sockFd, buffer, 1, 0);
		if (r == 0)
		{
			connected = false;
		}
		else if (r > 0)
		{
			if (buffer[0] != '\n')
				concatstr += buffer;
			else
			{
				concatstr += "\n";
				sendBack(concatstr);
				std::cout << concatstr <<std::endl;
				concatstr = "";
			}
		}
		else
		{
			close(_sockFd);
			throw std::invalid_argument("Error: recv failed");
		}
	}
	close(_sockFd);
	_sockFd = -1;
	std::cout << "\nBot stopped." << std::endl;
}
