#include <Server.hpp>

static bool	isInt(std::string str)
{
	int limit;
	errno = 0;
	char *end = NULL;

	limit = std::strtol(str.c_str(), &end, 10);
	if (*end != '\0' || limit > INT_MAX || errno == ERANGE)
		return false;
	return true;
}

void	Server::mode(t_commandArgs & cArgs)
{
	std::string 						word;
	std::string 						args;
	bool 								activate = true;
	bool 								hasSign = false;
	int 								sscount = 0;
	std::map<std::string, std::string>	changes;
	std::map<std::string, Channel>::iterator currentChannelIt;

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
				//std::cout << "word[i] :" << word[i] << std::endl;
				if (word[i] == '+' && i != word.size() - 1)
				{
					hasSign = true;
					if (i == 0 || (i > 0 && word[i - 1] != '+' && word[i - 1] != '-'))
						activate = true;
					else
						throw std::invalid_argument("Error: double +.");
				}
				else if (word[i] == '-' && i != word.size() - 1)
				{
					hasSign = true;
					if (i == 0 || (i > 0 && word[i - 1] != '+' && word[i - 1] != '-'))
						activate = false;
					else
						throw std::invalid_argument("Error: double -.");
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
						changes["+t"] = "";
					}
				}
				else if (word[i] == 'k')
				{
					if (activate)
					{
						if (*cArgs.sstream >> args)
						{
							//*cArgs._channels.addOperator(args);
							changes["+k"] = args;
						}
						else
						{
							;// server has to ignore if no argument is given.
						}
					}
					else
					{
						//*cArgs._channels.removeOperator(*cArgs.client);
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
						if (*cArgs.sstream >> args && isInt(args))
						{
							//*cArgs._channels.addOperator(args);
							changes["+l"] = args;
						}
						else
						{
							;// server has to ignore if no argument is given.
						}
					}
					else
					{
						//*cArgs._channels.removeOperator(*cArgs.client);
							changes["-l"] = "";
					}
				}
				else
					throw std::invalid_argument(" ERR_UMODEUNKNOWNFLAG (501)");
			}
		}
		sscount++;
	}
	if (sscount == 1)
		throw std::invalid_argument("RPL_CHANNELMODEIS (324) & RPL_CREATIONTIME (329)");

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
	//

	(void)cArgs.prefix;//supress
	(void)cArgs.hasPrefix;//supress
	return ;
}
