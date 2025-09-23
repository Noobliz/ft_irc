#include <Server.hpp>

static bool	parseNick(std::string nick)
{
	if (nick.length() > 9)
		return false;
	for (std::string::iterator it = nick.begin(); it != nick.end(); ++it)
	{
		if ((*it) == ',' || (*it) == '#' || (*it) == ' ' || ((*it) >= '\t' && (*it) <= '\r'))
			return false;
	}
	return true;
}

void	Server::nick(t_commandArgs & cArgs)
{
	std::string	nick;
	std::string	words;
	std::string	err_feedback;
	std::string	feedback;
	int			sscount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
			nick = words;
		else if (sscount > 0)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 1)
	{
		err_feedback = ERR_NONICKNAMEGIVEN;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}

	if (!parseNick(nick))
	{
		err_feedback = ERR_ERRONEUSNICKNAME(nick);
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: bad nickname (too long or invalid chars).");
	}
	else if (findClient(nick) != -2)
	{
		err_feedback = ERR_NICKNAMEINUSE(nick);
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: nickname already used.");
	}
	if (!cArgs.client->isAuth())
	{
		cArgs.client->setNickname(nick);
		cArgs.client->setNickValid(true);
	}
	else
	{
		feedback = NICK(cArgs.client->getNickname(), nick);
		if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		cArgs.client->setNickname(nick);
	}
}
