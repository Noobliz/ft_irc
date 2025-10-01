#include <Server.hpp>

void	Server::privmsg(t_commandArgs & cArgs)
{
	std::vector<std::string>	msgTarget;
	std::string		msg;
	std::string		words;
	std::string		err_feedback;
	std::streampos	ssPos;
	int				sscount = 0;

	while (*cArgs.sstream >> words)
	{
		if (sscount == 0)
		{
			std::stringstream targets(words);
			std::string target;
			while (std::getline(targets, target, ','))
				msgTarget.push_back(target);
			ssPos = cArgs.sstream->tellg();
		}
		else if (sscount == 1)
		{
			if (words[0] == ':')
			{
				msg = (cArgs.sstream->str()).substr(ssPos);
				std::stringstream msgSStream(msg);
				std::getline(msgSStream, msg, ':');
				std::getline(msgSStream, msg);
				sscount++;
				break;
			}
			msg = words;
		}
		else if(sscount > 1)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 2)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "PRIVMSG");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (cArgs.client->isAuth())
		doPrivateMsg(*cArgs.client, msgTarget, msg);
	else
	{
		err_feedback = ERR_NOTREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(err_feedback);
	}
}

int	Server::findClient(std::string nickname)
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.getNickname() == nickname)
			return it->second.getFD();
	}
	return (-2);
}

void	Server::doPrivateMsg(Client & client, std::vector<std::string> nick, std::string msg)
{
	std::map<std::string, Channel>::iterator	ite2;
	int			fd;
	std::string	feedback;

	if (msg.empty())
	{
		feedback = NO_TEXTOSEND(client.getNickname());
		if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		return ;
	}
	for(size_t i = 0; i < nick.size(); i++)
	{
		fd = findClient(nick[i]);
		ite2 = _channels.find(nick[i]);
		if(fd >= 0)
		{
			if (_clients[fd].isAuth())
			{
				feedback = PRIVMSG(client.getNickname(), client.getUserinfo().username, _clients[fd].getNickname(), msg);
				if (send(fd, feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
				feedback = "";
			}
		}
		else if (ite2 != _channels.end())
		{
			std::string	index = (*ite2).first;
			std::map<std::string, Client>	tmp;
			tmp = _channels[index].getConnectedClients();
			if (client.isInChan(nick[i]))
			{
				std::map<std::string, Client>::iterator ite = tmp.begin();
				for (; ite != tmp.end(); ++ite)
				{
					feedback = PRIVMSG(client.getNickname(), client.getUserinfo().username, nick[i], msg);
					if ((*ite).second.getNickname() != client.getNickname())
					{
						if (send((*ite).second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
							throw std::runtime_error("send() failed");
					}
					feedback = "";
				}
			}
			else
			{
				feedback = ERR_CANNOTSENDTOCHAN(client.getNickname(), nick[i]);
				if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
				feedback = "";
			}
		}
		else
		{
			//nosuchchan not used in the norm, only nosuchnick for both cases
			feedback = ERR_NOSUCHNICK(client.getNickname(), nick[i]);
			if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
			feedback = "";
		}
	}

}
