#include <Server.hpp>


void	Server::doQuit(Client & client, std::string msg)
{
	std::string	feedback;
	std::map<std::string, Channel>::iterator chanListIter = client.getChannels().begin();

	feedback = QUIT(client.getNickname(), client.getUserinfo().username, msg);

	for (; chanListIter != client.getChannels().end(); ++chanListIter)
	{
		std::map<std::string, Client>::iterator clientListIter = chanListIter->second.getConnectedClients().begin();

		for (; clientListIter != chanListIter->second.getConnectedClients().end(); ++clientListIter)
		{
			if (clientListIter->first != client.getNickname())
			{
				if (send(clientListIter->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
			}
		}
	}
	//! quitter proprement le client du server (epoll ctl + close client fd)
}

void	Server::quit(t_commandArgs & cArgs)
{
	std::streampos	ssPos;
	std::string	msg;
	std::string	words;
	std::string	err_feedback;
	int	sscount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
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
