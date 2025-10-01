#include <Server.hpp>

void	Server::doQuit(Client & client, std::string msg)
{
	std::string	feedback;
	feedback = QUIT(client.getNickname(), client.getUserinfo().username, msg);

	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		std::map<std::string, Client>::iterator findClient = it->second.getConnectedClients().find(client.getNickname());
		if (findClient != it->second.getConnectedClients().end())
		{
			findClient = it->second.getConnectedClients().begin();
			for (; findClient != it->second.getConnectedClients().end(); ++findClient)
			{
				if (findClient->first != client.getNickname())
				{
					if (send(findClient->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
						throw std::runtime_error("send() failed");
				}
			}
			it->second.getConnectedClients().erase(client.getNickname());
			it->second.getInvitedClient().erase(client.getNickname());
			it->second.getOperators().erase(client.getNickname());
			if (it->second.getConnectedClients().size() == 0)
			{
				_channels.erase(it->first);
				break;
			}
		}
	}
	int	tmpfd = client.getFD();
	if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, client.getFD(), NULL) == -1)
		throw std::invalid_argument("Error: epoll_ctl fail");
	_clients.erase(client.getFD());
	close(tmpfd);
}

void	Server::quit(t_commandArgs & cArgs)
{
	std::streampos	ssPos;
	std::string		msg;
	std::string		words;
	std::string		err_feedback;
	int				sscount = 0;

	while (*cArgs.sstream >> words)
	{
		if (sscount == 0)
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
		else if(sscount > 0)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 1)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "PART");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (cArgs.client->isAuth())
		doQuit(*cArgs.client, msg);
	else
	{
		err_feedback = ERR_NOTREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(err_feedback);
	}
}
