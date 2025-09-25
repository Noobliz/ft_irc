#include <Server.hpp>

void	Server::doTopic(t_commandArgs & cArgs, std::string & chan, std::string & msg)
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

	if (msg.empty())
	{
		if (iteChan->second.getTopic().empty())
		{
			feedback = RPL_NOTOPIC(cArgs.client->getNickname(), chan);
		}
		else
		{
			feedback = RPL_TOPIC(cArgs.client->getNickname(), chan, iteChan->second.getTopic());
		}
	}
	else
	{
		if (!iteChan->second.isTopicForAll() && !iteChan->second.isOperator(*cArgs.client))
		{
			feedback = ERR_CHANOPRIVSNEEDED(cArgs.client->getNickname(), chan);
			if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
			throw std::invalid_argument(feedback);
		}
		else
		{
			iteChan->second.setTopic(msg);
			feedback = TOPIC(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, chan, msg);
		}
	}
	std::map<std::string, Client>::iterator	iteClient = iteChan->second.getConnectedClients().begin();
	for (; iteClient != iteChan->second.getConnectedClients().end(); ++iteClient)
	{
		if (send(iteClient->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
	}
	std::cout << feedback << std::endl;
}

void	Server::topic(t_commandArgs & cArgs)
{
	std::string		channel;
	std::string		msg = "";
	std::string		words;
	std::string		err_feedback;
	int				sscount = 0;
	std::streampos	ssPos;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
		{
			if (words[0] != '#')
			{
				throw std::invalid_argument("Error: channel name has to start with #.");
			}
			channel = words;
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
				msg.append("\n");
				sscount++;
				break;
			}
			msg = words;
			msg.append("\n");
		}
		else if (sscount == 2)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount < 1)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "TOPIC");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (cArgs.client->isAuth())
		doTopic(cArgs, channel, msg);
	else
	{
		err_feedback = ERR_NOTREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(err_feedback);
	}
}
