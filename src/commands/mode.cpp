#include <Server.hpp>

static void	showModes(std::map<std::string, Channel>::iterator & currentChannelIt, t_commandArgs & cArgs)
{
	std::string modestring = "+";
	std::string mode_arguments = "";
	std::string feedback = "";

	if (currentChannelIt->second.isInInviteMode())
		modestring += "i";
	if (currentChannelIt->second.getPassword() != "")
	{
		modestring += "k";
		mode_arguments += currentChannelIt->second.getPassword() + " ";
	}
	if (currentChannelIt->second.isTopicForAll())
	{
		modestring += "t";
		if (currentChannelIt->second.getTopic() != "")
			mode_arguments += currentChannelIt->second.getTopic() + " ";
	}
	if (currentChannelIt->second.getUserLimit() != -1)
	{
		modestring += "l";
		std::ostringstream convert;
		convert << currentChannelIt->second.getUserLimit();
		mode_arguments += convert.str() + " ";
	}
	std::map<std::string, Client>::const_iterator it = currentChannelIt->second.getOpperators().begin();
	if (it != currentChannelIt->second.getOpperators().end())
	{
		modestring += "o";
		for (; it != currentChannelIt->second.getOpperators().end(); ++it)
			mode_arguments += it->second.getNickname() + ",";
		mode_arguments = mode_arguments.substr(0, mode_arguments.size() - 1);
	}
	feedback = RPL_CHANNELMODEIS(cArgs.client->getNickname(), currentChannelIt->first, modestring, mode_arguments);
	if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
		throw std::runtime_error("send() failed");
}

static void	mode_I(bool activate, std::map<std::string, Channel>::iterator & currentChannelIt)
{
	std::string feedback;

	if (activate)
	{
		currentChannelIt->second.setInviteMode(true);
		feedback = RPL_MODECHANGED1(currentChannelIt->first, "+i");
		for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
			it != currentChannelIt->second.getConnectedClients().end(); ++it)
		{
			if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
	else
	{
		currentChannelIt->second.setInviteMode(false);
		feedback = RPL_MODECHANGED1(currentChannelIt->first, "-i");
		for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
			it != currentChannelIt->second.getConnectedClients().end(); ++it)
		{
			if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
}

static void	mode_T(bool activate, std::map<std::string, Channel>::iterator & currentChannelIt)
{
	std::string feedback;

	if (activate)
	{
		currentChannelIt->second.setTopicForAll(true);
		feedback = RPL_MODECHANGED1(currentChannelIt->first, "+t");
		for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
			it != currentChannelIt->second.getConnectedClients().end(); ++it)
		{
			if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
	else
	{
		currentChannelIt->second.setTopicForAll(false);
		feedback = RPL_MODECHANGED1(currentChannelIt->first, "-t");
		for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
			it != currentChannelIt->second.getConnectedClients().end(); ++it)
		{
			if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
}

static void	mode_K(bool activate, std::map<std::string, Channel>::iterator & currentChannelIt, t_commandArgs & cArgs)
{
	std::string feedback;
	std::string args;

	if (activate)
	{
		if (*cArgs.sstream >> args) //? if there is an argument left in the stringstream
		{
			currentChannelIt->second.setPassword(args);
			feedback = RPL_MODECHANGED2(currentChannelIt->first, "+k", args);
			for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
				it != currentChannelIt->second.getConnectedClients().end(); ++it)
			{
				if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
			}
		}
		else
		{
			;//? server has to ignore if no argument is given.
		}
	}
	else
	{
		currentChannelIt->second.setPassword("");
		feedback = RPL_MODECHANGED1(currentChannelIt->first, "-k");
		for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
			it != currentChannelIt->second.getConnectedClients().end(); ++it)
		{
			if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
}

static void	mode_O(bool activate, std::map<std::string, Channel>::iterator & currentChannelIt, t_commandArgs & cArgs)
{
	std::string feedback;
	std::string args;

	if (*cArgs.sstream >> args) //? if there is an argument left in the stringstream
	{
		std::map<std::string, Client>::iterator target;
		target = currentChannelIt->second.getConnectedClients().find(args);
		if (target != currentChannelIt->second.getConnectedClients().end())
		{
			if (activate)
			{
				currentChannelIt->second.addOperator(target->second);
				feedback = RPL_MODECHANGED2(currentChannelIt->first, "+o", args);
				for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
					it != currentChannelIt->second.getConnectedClients().end(); ++it)
				{
					if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
						throw std::runtime_error("send() failed");
				}
			}
			else
			{
				currentChannelIt->second.removeOperator(target->second);
				feedback = RPL_MODECHANGED2(currentChannelIt->first, "-o", args);
				for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
					it != currentChannelIt->second.getConnectedClients().end(); ++it)
				{
					if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
						throw std::runtime_error("send() failed");
				}
			}
		}
		else //? if User doesn't exist in this channel
		{
			feedback = ERR_USERNOTINCHANNEL(cArgs.client->getNickname(), args, currentChannelIt->first);
			if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
	else
	{
		;//? server has to ignore if no argument is given.
	}
}

static void	mode_L(bool activate, std::map<std::string, Channel>::iterator & currentChannelIt, t_commandArgs & cArgs)
{
	std::string feedback;
	std::string args;

	if (activate)
	{
		if (*cArgs.sstream >> args) //? if there is an argument left in the stringstream
		{
			int limit;
			errno = 0;
			char *end = NULL;

			limit = std::strtol(args.c_str(), &end, 10); //? converts to int
			if (*end == '\0' || errno == ERANGE || limit > MAX_CLIENT)
			{
				currentChannelIt->second.setUserLimit(limit);
				feedback = RPL_MODECHANGED2(currentChannelIt->first, "+l", args);
				for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
					it != currentChannelIt->second.getConnectedClients().end(); ++it)
				{
					if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
						throw std::runtime_error("send() failed");
				}
			}
		}
		else
		{
			;//? server has to ignore if no argument is given.
		}
	}
	else
	{
		currentChannelIt->second.setUserLimit(-1);
		feedback = RPL_MODECHANGED1(currentChannelIt->first, "-l");
		for(std::map<std::string, Client>::iterator it = currentChannelIt->second.getConnectedClients().begin();
			it != currentChannelIt->second.getConnectedClients().end(); ++it)
		{
			if (send(it->second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
		}
	}
}

void	Server::mode(t_commandArgs & cArgs)
{
	std::string	word;
	std::string	args;
	std::string	feedback;
	bool		activate = true; //? + or -
	bool		hasSign = false; //? if a + or - has been found
	int			sscount = 0; //? number of stringstream done on the command
	std::map<std::string, Channel>::iterator	currentChannelIt; //? the iterator pointing at the targetted channel

	//?Check if User is authentified
	if (!cArgs.client->isAuth())
	{
        feedback = ERR_NOTREGISTERED;
        if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
            throw std::runtime_error("send() failed");
        throw std::invalid_argument(feedback);
	}
	while (*cArgs.sstream >> word)
	{
		//? Case MODE #channel (...)
		if (sscount == 0)
		{
			currentChannelIt = _channels.find(word);
			//?Check if channel exists
			if (word[0] != '#' || currentChannelIt == _channels.end())
			{
				feedback = ERR_NOSUCHCHANNEL(cArgs.client->getNickname(), word);
				if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
				throw std::invalid_argument(feedback);
			}
			//?Check if User is operator
			if (!currentChannelIt->second.isOperator(*cArgs.client))
			{
				feedback = ERR_CHANOPRIVSNEEDED(cArgs.client->getNickname(), currentChannelIt->first);
				if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
				throw std::invalid_argument(feedback);
			}
		}
		//? Case MODE #channel <modes>
		else if (sscount == 1)
		{
			for (size_t i = 0; i < word.size(); i++)
			{
				if (word[i] == '+')
				{
					hasSign = true;
					activate = true;
				}
				else if (word[i] == '-')
				{
					hasSign = true;
					activate = false;
				}
				else if (word[i] == 'i' && hasSign)
					mode_I(activate, currentChannelIt);
				else if (word[i] == 't' && hasSign)
					mode_T(activate, currentChannelIt);
				else if (word[i] == 'k' && hasSign)
					mode_K(activate, currentChannelIt, cArgs);
				else if (word[i] == 'o' && hasSign)
					mode_O(activate, currentChannelIt, cArgs);
				else if (word[i] == 'l' && hasSign)
					mode_L(activate, currentChannelIt, cArgs);
				else
				{
					feedback = ERR_UNKNOWNMODE(cArgs.client->getNickname(), word[i]);
					if (send(cArgs.client->getFD(), feedback.c_str(), feedback.length(), 0) == -1)
						throw std::runtime_error("send() failed");
				}
			}
		}
		sscount++;
	}
	//? Case MODE #channel 
	if (sscount == 1)
		showModes(currentChannelIt, cArgs);
}
