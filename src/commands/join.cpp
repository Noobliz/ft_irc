#include <Server.hpp>

void	Server::doJoin(std::map<std::string, std::string> chanPwPair, bool resetUserChans, t_commandArgs cArgs)
{
	(void)resetUserChans; //!handle reset user chans ?
	std::string	feedbackToAll;
	std::string	feedbackNameList;
	std::string	feedback;

	for (std::map<std::string, std::string>::iterator it = chanPwPair.begin(); it != chanPwPair.end(); ++it)
	{
		std::map<std::string, Channel>::iterator cit = _channels.find(it->first);
		if (cit != _channels.end())
		{
			if (_channels[it->first].checkPassword(it->second) == false)
			{
				feedback = ERR_BADCHANNELKEY(cArgs.client->getNickname(), _channels[it->first].getName());
				if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
			}
			else if (_channels[it->first].isFull())
			{
				feedback = ERR_CHANNELISFULL(cArgs.client->getNickname(), _channels[it->first].getName());
				if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
			}
			else if (_channels[it->first].isInInviteMode() && !_channels[it->first].isInvited(*cArgs.client))
			{
				feedback = ERR_INVITEONLYCHAN(cArgs.client->getNickname(), _channels[it->first].getName());
				if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
			}
			else
			{
				_channels[it->first].addClient(*cArgs.client);
				_channels[it->first].removeInvited(*cArgs.client);
				_channels[it->first].removeOperator(*cArgs.client);
				cArgs.client->addChan(it->first, _channels[it->first]);
				for (std::map<std::string, Client>::iterator clientit = _channels[it->first].getConnectedClients().begin(); clientit != _channels[it->first].getConnectedClients().end(); ++clientit)
				{
					feedbackToAll = JOIN(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, it->first);
					if (send((*clientit).second.getFD(), feedbackToAll.c_str(), feedbackToAll.length(), 0) == -1)
						throw std::runtime_error("send() failed");
					if (_channels[it->first].isOperator(clientit->second))
					{
						feedbackNameList += "@" + clientit->second.getNickname() + " ";
					}
					else
					{
						feedbackNameList += clientit->second.getNickname() + " ";
					}
				}
				feedback = RPL_NAMREPLY(cArgs.client->getNickname(), it->first);
				feedback += feedbackNameList + END;
				feedback += RPL_ENDOFNAMES(cArgs.client->getNickname(), it->first);
				if (_channels[it->first].getTopic() == "")
					feedback += RPL_NOTOPIC(cArgs.client->getNickname(), it->first);
				else
					feedback += RPL_TOPIC(cArgs.client->getNickname(), it->first, _channels[it->first].getTopic());
				if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
			}
		}
		else
		{
			Channel newChan(*cArgs.client, it->first, it->second);
			_channels[it->first] = newChan;
			_channels[it->first].addClient(*cArgs.client);
			cArgs.client->addChan(it->first, _channels[it->first]);
			feedback = JOIN(cArgs.client->getNickname(), cArgs.client->getUserinfo().username, it->first);
			feedback += RPL_NAMREPLY(cArgs.client->getNickname(), it->first) + "@" + cArgs.client->getNickname() + END;
			feedback += RPL_ENDOFNAMES(cArgs.client->getNickname(), it->first);
			feedback += RPL_NOTOPIC(cArgs.client->getNickname(), it->first);
			if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
}

void	Server::join(t_commandArgs & cArgs)
{
	std::map<std::string, std::string>	channelPw;
	std::stack<std::string>				passwords;
	std::string							words;
	std::string							err_feedback;
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
				if (channelName.length() > 200)
				{
					throw std::invalid_argument("Error: max 200 char for channel name.");
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
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "JOIN");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (cArgs.client->isAuth())
		doJoin(channelPw, resetUserChans, cArgs);
	else
	{
		err_feedback = ERR_NOTREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument(err_feedback);
	}
}
