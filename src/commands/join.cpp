#include <Server.hpp>

void	Server::doJoin(std::map<std::string, std::string> chanPwPair, bool resetUserChans, t_commandArgs cArgs)
{
	(void)resetUserChans; //!handle reset user chans
	std::string	feedbackToAll;
	std::string	feedbackNameList;
	std::string	feedback;

	for (std::map<std::string, std::string>::iterator it = chanPwPair.begin(); it != chanPwPair.end(); ++it)
	{
		std::map<std::string, Channel>::iterator cit = _channels.find((*it).first);
		if (cit != _channels.end())
		{
			//! handle channel user limit
			if (_channels[(*it).first].checkPassword((*it).second))
			{
				//? attention, addClient ne rajoutera pas de client s'il est pas invité et que le server est en inviteMode
				_channels[(*it).first].addClient(*cArgs.client);
				cArgs.client->addChan((*it).first, _channels[(*it).first]);
				for (std::map<std::string, Client>::iterator clientit = _channels[(*it).first].getConnectedClients().begin(); clientit != _channels[(*it).first].getConnectedClients().end(); ++clientit)
				{
					feedbackToAll = JOIN(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, (*it).first);
					send((*clientit).second.getFD(), feedbackToAll.c_str(), feedbackToAll.length(), 0);
					feedbackToAll = "";
					feedbackNameList += UINFO((*clientit).second.getNickname(), (*clientit).second.getUserinfo().username) + " ";
				}
				feedback = RPL_NAMREPLY(cArgs.client->getNickname(), (*it).first);
				feedback += feedbackNameList + END;
				feedback += RPL_ENDOFNAMES(cArgs.client->getNickname(), (*it).first);
				feedback += RPL_NOTOPIC(cArgs.client->getNickname(), (*it).first);
				send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0);
				feedback = "";
				feedbackNameList = "";
			}
			else
			{
				//! err bad pass
			}
		}
		else
		{
			Channel newChan(*cArgs.client, (*it).first, (*it).second);
			_channels[(*it).first] = newChan;
			_channels[(*it).first].addClient(*cArgs.client);
			_channels[(*it).first].addOperator(*cArgs.client);
			cArgs.client->addChan((*it).first, _channels[(*it).first]);
			feedback = JOIN(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, (*it).first);
			feedback += RPL_NAMREPLY(cArgs.client->getNickname(), (*it).first) + UINFO(cArgs.client->getNickname(), cArgs.client->getUserinfo().username) + END;
			feedback += RPL_ENDOFNAMES(cArgs.client->getNickname(), (*it).first);
			if (_channels[(*it).first].getTopic() == "")
				feedback += RPL_NOTOPIC(cArgs.client->getNickname(), (*it).first);
			else
				feedback += RPL_TOPIC(cArgs.client->getNickname(), (*it).first, _channels[(*it).first].getTopic());
			send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0);
			feedback = "";
		}
	}
	/* accusé de réception ? */
}

void	Server::join(t_commandArgs & cArgs)
{
	std::map<std::string, std::string>	channelPw;
	std::stack<std::string>				passwords;
	std::string							words;
	bool	resetUserChans = false;
	int		sscount = 0;
	int		channelCount = 0;
	int		passwordCount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (words == "0" && sscount != 1)
			resetUserChans = true;
		else if (resetUserChans)
		{
			throw std::invalid_argument("Error: too many arguments.");

		}
		else if (sscount == 0)
		{
			std::stringstream	channels(words);
			std::string			channelName;
			while (std::getline(channels, channelName, ','))
			{
				if (channelName[0] != '#')
				{
					throw std::invalid_argument("Error: channel name has to start with #.");
				}
				channelCount++;
				std::cout << "channel" << channelCount << ": " << channelName << std::endl;
				channelPw[channelName] = "";
			}
		}
		else if (sscount == 1)
		{
			std::stringstream pwList(words);
			std::string password;
			while (std::getline(pwList, password, ','))
			{
				passwordCount++;
				std::cout << "password" << passwordCount << ": " << password << std::endl;
				passwords.push(password);
			}
			if (passwordCount > channelCount)
			{
				throw std::invalid_argument("Error: More passwords than channels.");

			}
			std::map<std::string, std::string>::reverse_iterator it = channelPw.rbegin();
			while (channelCount > passwordCount)
			{
				it++;
				channelCount--;
			}
			while (it != channelPw.rend())
			{
				it->second = passwords.top();
				passwords.pop();
				it++;
			}
		}
		sscount++;
	}
	if (sscount < 1)
		throw std::invalid_argument("Error: not enough arguments.");
	// std::cout << "Debug infos:" << std::endl;
	// std::cout << "Map:" << std::endl;
	// for (std::map<std::string, std::string>::iterator it = channelPw.begin(); it != channelPw.end(); ++it)
	// {
	// 	std::cout << it->first << " -> " << it->second <<std::endl;
	// }
	// std::cout << "Reset User Channels: " << resetUserChans << std::endl;
	// std::cout << std::endl;

	doJoin(channelPw, resetUserChans, cArgs);
	return ;
}
