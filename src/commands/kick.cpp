#include <Server.hpp>

#include <Server.hpp>

void	Server::kick(t_commandArgs & cArgs)
{
	std::vector<std::string> targets;
	std::string channel;
	std::string msg = "";
	std::string words;
	int sscount = 0;
	std::streampos ssPos;
	std::string err_feedback;

	while (*cArgs.sstream >> words)
	{
		if (sscount == 0)
		{
			if (words[0] != '#')
			{
				err_feedback = ERR_BADCHANMASK(cArgs.client->getNickname(), words);
				if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
				throw std::invalid_argument("Error: channel name has to start with #.");
			}
			channel = words;
		}
		else if (sscount == 1)
		{
			std::stringstream target(words);
			while (std::getline(target, words, ','))
				targets.push_back(words);
			ssPos = cArgs.sstream->tellg();
		}
		else if (sscount == 2)
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
		else if (sscount == 3)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount < 2)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "INVITE");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}

	if (cArgs.client->isAuth())
		doKick(*cArgs.client, targets, channel, msg);
	else
	{
		err_feedback = ERR_NOTREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(err_feedback);
	}
}

void	Server::doKick(Client & client, std::vector<std::string> const &target, std::string const &chan, std::string const &msg)
{
	std::map<std::string, Channel>::iterator	iteChan;
	std::string	feedback;

	iteChan = _channels.find(chan);
	if (iteChan == _channels.end())
	{
		feedback = ERR_NOSUCHCHANNEL(client.getNickname(), chan);
		if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: channel not found");
	}

	if (!client.isInChan(chan))
	{
		feedback = ERR_NOTONCHANNEL(client.getNickname(), chan);
		if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: client not in channel");
	}

	if (!(*iteChan).second.isOperator(client))
	{
		feedback = ERR_CHANOPRIVSNEEDED(client.getNickname(), chan);
		if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: client has not enough privileges");
	}

	for(size_t i = 0; i < target.size(); i++)
	{
		int clientfd = findClient(target[i]);
		if (clientfd == -2)
		{
			feedback = ERR_NOSUCHNICK(client.getNickname(), target[i]);
			if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
			throw std::invalid_argument("Error: target client not found");
		}
		if (_clients[clientfd].isInChan(chan) == false)
		{
			feedback = ERR_USERNOTINCHANNEL(client.getNickname(), target[i], chan);
			if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
			throw std::invalid_argument("Error: target not in channel");
		}
		std::map<std::string, Client> &connectedClients = iteChan->second.getConnectedClients();
		std::map<std::string, Client>::iterator ite2 = connectedClients.begin();

		feedback = KICK(client.getNickname(), client.getUserinfo().username, chan, target[i], msg);

		for (; ite2 != connectedClients.end(); ++ite2)
		{
			if (send(ite2->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
		_clients[clientfd].removeChan(chan);
		iteChan->second.removeClient(target[i]);
		if (iteChan->second.getConnectedClients().size() == 0)
			_channels.erase(iteChan->first);
	}
}
