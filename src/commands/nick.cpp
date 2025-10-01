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
		if (cArgs.client->isAuth())
		{
			std::string	welcome = WELCOME(cArgs.client->getNickname());
			if (send(cArgs.client->getFD(), welcome.c_str(), welcome.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
	else
	{
		std::string	oldnick = cArgs.client->getNickname();
		cArgs.client->setNickname(nick);

		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		{
			std::map<std::string, Client>::iterator findClient = it->second.getConnectedClients().find(oldnick);
			if (findClient != it->second.getConnectedClients().end())
			{
				findClient = it->second.getConnectedClients().begin();
				for (; findClient != it->second.getConnectedClients().end(); ++findClient)
				{
					if (findClient->first == oldnick)
					{
						feedback = NICK(oldnick, nick);
					}
					else
					{
						feedback = NICK(UINFO(oldnick, cArgs.client->getUserinfo().username), nick);
					}
					if (send(findClient->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
						throw std::runtime_error("send() failed");
				}
				if (it->second.getConnectedClients().erase(oldnick))
					it->second.getConnectedClients()[nick] = *cArgs.client;
				if (it->second.getInvitedClient().erase(oldnick))
					it->second.getInvitedClient()[nick] = *cArgs.client;
				if (it->second.getOperators().erase(oldnick))
					it->second.getOperators()[nick] = *cArgs.client;
			}
		}
	}
}
