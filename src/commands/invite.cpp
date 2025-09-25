#include <Server.hpp>

void	Server::doInvite(Client & client, std::string const & target, std::string const & chan)
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

	if (findClient(target) == -2)
	{
		feedback = ERR_NOSUCHNICK(client.getNickname(), target);
		if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: target client not found");
	}

	if (_clients[findClient(target)].isInChan(chan))
	{
		feedback = ERR_USERONCHANNEL(client.getNickname(), target, chan);
		if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: target client already in channel");
	}

	std::map<int, Client>::iterator iteTarget = _clients.find(findClient(target));
	(*iteChan).second.inviteClient((*iteTarget).second);
	feedback = INVITE(client.getNickname(), client.getUserinfo().username, target, chan);
	if (send(findClient(target), feedback.c_str(), feedback.length(), 0) == -1)
		throw std::runtime_error("send() failed");

}

void	Server::invite(t_commandArgs & cArgs)
{
	std::string	target;
	std::string	channel;
	std::string	words;
	std::string	err_feedback;
	int			sscount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
			target = words;
		else if (sscount == 1)
		{
			if (words[0] != '#')
				throw std::invalid_argument("Error: channel name has to start with #.");
			channel = words;
		}
		else if (sscount > 1)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 2)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "INVITE");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (cArgs.client->isAuth())
		doInvite(*cArgs.client, target, channel);
    else
    {
        err_feedback = ERR_NOTREGISTERED;
        if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
            throw std::runtime_error("send() failed");
        throw std::invalid_argument(err_feedback);
    }
}
