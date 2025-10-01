#include <Server.hpp>

void	Server::user(t_commandArgs & cArgs)
{
	t_userinfos		userinfos;
	std::string		words;
	std::string		err_feedback;
	int				sscount = 0;
	std::streampos	ssPos;

	while (*cArgs.sstream >> words)
	{
		if (sscount == 0)
			userinfos.username = words;
		else if (sscount == 1)
			userinfos.hostname = words;
		else if (sscount == 2)
		{
			userinfos.servername = words;
			ssPos = cArgs.sstream->tellg();
		}
		else if (sscount == 3)
		{
			if (words[0] == ':')
			{
				userinfos.realname = (cArgs.sstream->str()).substr(ssPos);
				std::stringstream msgSStream(userinfos.realname);
				std::getline(msgSStream, userinfos.realname, ':');
				std::getline(msgSStream, userinfos.realname);
				userinfos.realname.append("\n");
				sscount++;
				break;
			}
			userinfos.realname = words;
			userinfos.realname.append("\n");
		}
		else if(sscount > 3)
		{
			throw std::invalid_argument("Error: too many arguments.");
		}
		sscount++;
	}
	if (sscount != 4)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "USER");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}

	if (!cArgs.client->isAuth())
	{
		cArgs.client->setUserinfo(userinfos);
		cArgs.client->setUserValid(true);
		if (cArgs.client->isAuth())
		{
			std::string	welcome = WELCOME(cArgs.client->getNickname());
			if (send(cArgs.client->getFD(), welcome.c_str(), welcome.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
	else
	{
		err_feedback = ERR_ALREADYREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
	}
}
