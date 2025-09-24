#include <Server.hpp>

// static bool	isInt(std::string str)
// {
// 	int limit;
// 	errno = 0;
// 	char *end = NULL;

// 	limit = std::strtol(str.c_str(), &end, 10);
// 	if (*end != '\0' || limit > INT_MAX || errno == ERANGE)
// 		return false;
// 	return true;
// }

/*
!ERR_UNKNOWNMODE (472)

!  "<client> <modechar> :is unknown mode char to me"

!Indicates that a mode character used by a client is not recognized by the server. The text used in the last param of this message may vary.
*/

void	Server::mode(t_commandArgs & cArgs)
{
	std::string	word;
	std::string	args;
	bool		activate = true;
	bool		hasSign = false;
	int			sscount = 0;
	std::map<std::string, std::string>			changes;
	std::map<std::string, Channel>::iterator	currentChannelIt;

	while (*cArgs.sstream >> word)
	{
		if (sscount == 0)
		{
			currentChannelIt = _channels.find(word);
			if (word[0] != '#' || currentChannelIt == _channels.end())
				throw std::invalid_argument("ERR_NOSUCHCHANNEL (403)");
		}
		else if (sscount == 1)
		{
			if (!currentChannelIt->second.isOperator(*cArgs.client))
				throw std::invalid_argument("ERR_CHANOPRIVSNEEDED (482)");
			for (size_t i = 0; i < word.size(); i++)
			{
				if (word[i] == '+')
				{
					if (i == word.size() - 1)
					{
						sscount++;
						break;
					}
					hasSign = true;
					activate = true;
				}
				else if (word[i] == '-')
				{
					if (i == word.size() - 1)
					{
						sscount++;
						break;
					}
					hasSign = true;
					activate = false;
				}
				else if (word[i] == 'i')
				{
					if (activate)
					{
						currentChannelIt->second.setInviteMode(true);
						changes["+i"] = "";
					}
					else
					{
						currentChannelIt->second.setInviteMode(false);
						changes["-i"] = "";
					}
				}
				else if (word[i] == 't')
				{
					if (activate)
					{
						currentChannelIt->second.setTopicForAll(true);
						changes["+t"] = "";
					}
					else
					{
						currentChannelIt->second.setTopicForAll(false);
						changes["-t"] = "";
					}
				}
				else if (word[i] == 'k')
				{
					if (activate)
					{
						if (*cArgs.sstream >> args)
						{
							currentChannelIt->second.setPassword(args);
							changes["+k"] = args;
						}
						else
						{
							;// server has to ignore if no argument is given.
						}
					}
					else
					{
						currentChannelIt->second.setPassword("");
						changes["-k"] = "";
					}
				}
				else if (word[i] == 'o')
				{
					if (*cArgs.sstream >> args)
					{
						std::map<std::string, Client>::iterator target;
						target = currentChannelIt->second.getConnectedClients().find(args);
						if (target != currentChannelIt->second.getConnectedClients().end())
						{
							if (activate)
							{
								currentChannelIt->second.addOperator(target->second);
								changes["+o"] = args;
							}
							else
							{
								currentChannelIt->second.removeOperator(target->second);
								changes["-o"] = args;
							}
						}
						else
						{
							std::cout << "ERR_USERSDONTMATCH" << std::endl;
							;//ERR_USERSDONTMATCH ??
						}
					}
					else
					{
						;// server has to ignore if no argument is given.
					}
				}
				else if (word[i] == 'l')
				{
					if (activate)
					{
						if (*cArgs.sstream >> args)
						{
							int limit;
							errno = 0;
							char *end = NULL;

							limit = std::strtol(args.c_str(), &end, 10);
							if (*end == '\0' || errno == ERANGE || limit > MAX_CLIENT)
							{
								currentChannelIt->second.setUserLimit(limit);
								changes["+l"] = args;
							}
						}
						else
						{
							;// server has to ignore if no argument is given.
						}
					}
					else
					{
						currentChannelIt->second.setUserLimit(-1);
						changes["-l"] = "";
					}
				}
				else
					std::cout << "ERR_UNKNOWNMODE (472)" << std::endl;
			}
		}
		sscount++;
	}
	if (sscount == 1)
	{
		//!donner les infos au client
		throw std::invalid_argument("RPL_CHANNELMODEIS (324) & RPL_CREATIONTIME (329)");
	}

	//DEBUG
	std::cout << "MODE " << currentChannelIt->first << " ";
	for (std::map<std::string, std::string>::iterator it = changes.begin(); it != changes.end(); ++it)
	{
		std::cout << it->first;
	}
	for (std::map<std::string, std::string>::iterator it = changes.begin(); it != changes.end(); ++it)
	{
		std::cout << " ";
		std::cout << it->second;
	}
	std::cout << std::endl;
	//

}
