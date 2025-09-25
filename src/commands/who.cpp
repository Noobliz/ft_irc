#include <Server.hpp>

void	Server::doWho(std::string & chan, t_commandArgs & cArgs)
{
	std::map<std::string, Channel>::iterator	iteChan;
	std::string	feedback;

	iteChan = _channels.find(chan);
	if (iteChan == _channels.end())
	{
		feedback = ERR_NOSUCHCHANNEL(cArgs.client->getNickname(), chan);
		if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(feedback);
	}

	if (!cArgs.client->isInChan(chan))
	{
		feedback = ERR_NOTONCHANNEL(cArgs.client->getNickname(), chan);
		if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(feedback);
	}

	std::map<std::string, Client>::iterator	iteClient = iteChan->second.getConnectedClients().begin();
	for (; iteClient != iteChan->second.getConnectedClients().end(); ++iteClient)
	{
		if (iteChan->second.isOperator(iteClient->second))
			feedback += RPL_WHOREPLY_OPE(cArgs.client->getNickname(), iteClient->second.getUserinfo().username, iteClient->second.getNickname(), chan);
		else
			feedback += RPL_WHOREPLY(cArgs.client->getNickname(), iteClient->second.getUserinfo().username, iteClient->second.getNickname(), chan);
	}
	feedback += RPL_ENDOFWHO(cArgs.client->getNickname(), chan);
	if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
		throw std::runtime_error("send() failed");
}

void	Server::who(t_commandArgs & cArgs)
{
	std::string	target;
	std::string	channel;
	std::string	words;
	std::string	err_feedback;
	int			sscount = 0;

	while (*cArgs.sstream >> words)
	{
		if (sscount == 0)
		{
			if (words[0] != '#')
				throw std::invalid_argument("Error: channel name has to start with #.");
			channel = words;
		}
		else if (sscount > 1)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 1)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "WHO");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (cArgs.client->isAuth())
		doWho(channel, cArgs);
	else
	{
		err_feedback = ERR_NOTREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(err_feedback);
	}
}
