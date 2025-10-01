#include <Server.hpp>

void	Server::doPart(Client & client, std::vector<std::string> chans, std::string msg)
{
	std::map<std::string, Channel>::iterator	iteChan;
	std::string	feedback;

	for (std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); ++it)
	{
		iteChan = _channels.find((*it));
		if (iteChan == _channels.end())
		{
			feedback = ERR_NOSUCHCHANNEL(client.getNickname(), (*it));
			if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
			throw std::invalid_argument(feedback);
		}

		if (!client.isInChan((*it)))
		{
			feedback = ERR_NOTONCHANNEL(client.getNickname(), (*it));
			if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
			throw std::invalid_argument(feedback);
		}

		std::map<std::string, Client> &connectedClients = iteChan->second.getConnectedClients();
		std::map<std::string, Client>::iterator ite2 = connectedClients.begin();

		feedback = PART(client.getNickname(), client.getUserinfo().username, (*it), msg);
		for (; ite2 != connectedClients.end(); ++ite2)
		{
			if (send(ite2->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
		_clients[client.getFD()].removeChan((*it));
		iteChan->second.removeClient(client.getNickname());
		if (iteChan->second.getConnectedClients().size() == 0)
			_channels.erase(iteChan->first);
	}
}

void	Server::part(t_commandArgs & cArgs)
{
	std::vector<std::string>	chans;
	std::streampos	ssPos;
	std::string		msg;
	std::string		words;
	std::string		err_feedback;
	int				sscount = 0;

	while (*cArgs.sstream >> words)
	{
		if (sscount == 0)
		{
			std::stringstream	targets(words);
			std::string			target;
			while (std::getline(targets, target, ','))
			{
				if (target[0] != '#')
				{
					throw std::invalid_argument("Error: channel name has to start with #.");
				}
				chans.push_back(target);
			}
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
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "PART");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (cArgs.client->isAuth())
		doPart(*cArgs.client, chans, msg);
	else
	{
		err_feedback = ERR_NOTREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(err_feedback);
	}
}
