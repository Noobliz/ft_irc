#include <Server.hpp>

void	Server::user(t_commandArgs & cArgs)
{
	t_userinfos userinfos;
	std::string words;
	int sscount = 0;
	std::streampos ssPos;


	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
			userinfos.username = words;
		else if (sscount == 1)
			userinfos.hostname = words;
		else if (sscount == 2)
		{
			userinfos.servername = words;
			ssPos = cArgs.sstream->tellg();
		}
		else if (sscount == 3)
		{
			if (words[0] == ':')
			{
				userinfos.realname = (cArgs.sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
				std::stringstream msgSStream(userinfos.realname);
				std::getline(msgSStream, userinfos.realname, ':');
				std::getline(msgSStream, userinfos.realname);
				userinfos.realname.append("\n");
				sscount++;
				break;
			}
			userinfos.realname = words;
			userinfos.realname.append("\n");
		}
		else if(sscount > 3)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 4)
		throw std::invalid_argument("Error: not enough arguments.");
	// std::cout << "Debug infos:" << std::endl;
	// std::cout << "username:" << userinfos.username << std::endl;
	// std::cout << "hostname:" << userinfos.hostname << std::endl;
	// std::cout << "servername:" << userinfos.servername << std::endl;
	// std::cout << "realname:" << userinfos.realname << std::endl;
	// std::cout << std::endl;
	//! attention parsing char interdits/speciaux
	cArgs.client->setUserinfo(userinfos);
	cArgs.client->setUserValid(true);
	//doUser(userinfos, hasPrefix, prefix);
	return ;
}

void	Server::topic(t_commandArgs & cArgs)
{
	std::string channel;
	std::string msg = "";
	std::string words;
	int sscount = 0;
	std::streampos ssPos;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
		{
			if (words[0] != '#')
				throw std::invalid_argument("Error: channel name has to start with #.");
			channel = words;
			ssPos = cArgs.sstream->tellg();
		}
		else if (sscount == 1)
		{
			if (words[0] == ':')
			{
				msg = (cArgs.sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
		throw std::invalid_argument("Error: not enough arguments.");
	std::cout << "Debug infos:" << std::endl;
	std::cout << "Channel:" << channel << std::endl;
	std::cout << "New topic:" << msg << std::endl;
	std::cout << std::endl;
	(void)cArgs.prefix;//supress
	(void)cArgs.hasPrefix;//supress
	//doTopic(channel, msg, hasPrefix, prefix);
	return ;
}

void	Server::privmsg(t_commandArgs & cArgs)
{
	std::vector<std::string> msgTarget;
	std::string msg;
	std::string words;
	int sscount = 0;
	std::streampos ssPos;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
		{
			std::stringstream targets(words);
			std::string target;
			while (std::getline(targets, target, ','))
				msgTarget.push_back(target);
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
		else if(sscount > 1)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 2)
		throw std::invalid_argument("Error: not enough arguments.");
	// std::cout << "Debug infos:" << std::endl;
	// std::cout << "vector:" << std::endl;
	// for (std::vector<std::string>::iterator it = msgTarget.begin(); it != msgTarget.end(); ++it)
	// {
	//     std::cout << *it << std::endl;
	// }
	// std::cout << "Msg:" << msg << std::endl;
	// std::cout << std::endl;
	if (!cArgs.client->isAuth())
	{
		throw std::invalid_argument("Error: client not authentified.");
	}
	privateMsg(*cArgs.client, msgTarget, msg);
}

void	Server::pass(t_commandArgs & cArgs)
{
	std::string password;
	std::string words;
	int sscount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
			password = words;
		else if (sscount > 0)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 1)
		throw std::invalid_argument("Error: not enough arguments.");
	// std::cout << "Debug infos:" << std::endl;
	// std::cout << "Password:" << password << std::endl;
	// std::cout << std::endl;
	if (_password == password)
		cArgs.client->setPassValid(true);
}

void	Server::nick(t_commandArgs & cArgs)
{
	std::string nick;
	std::string words;
	int sscount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
			nick = words;
		else if (sscount > 0)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 1)
		throw std::invalid_argument("Error: not enough arguments.");
	// std::cout << "Debug infos:" << std::endl;
	// std::cout << "Nick:" << nick << std::endl;
	// std::cout << std::endl;

	//! attention au parsing des char speciaux / interdits

	if (findClient(nick) != -2)
	{
		throw std::invalid_argument("Error: nickname already used.");
	}
	cArgs.client->setNickname(nick);
	cArgs.client->setNickValid(true);
	return ;
}

void	Server::kick(t_commandArgs & cArgs)
{
	std::string target;
	std::string channel;
	std::string msg = "";
	std::string words;
	int sscount = 0;
	std::streampos ssPos;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
		{
			if (words[0] != '#')
				throw std::invalid_argument("Error: channel name has to start with #.");
			channel = words;
		}
		else if (sscount == 1)
		{
			target = words;
			ssPos = cArgs.sstream->tellg();
		}
		else if (sscount == 2)
		{
			if (words[0] == ':')
			{
				msg = (cArgs.sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
		else if (sscount == 3)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount < 2)
		throw std::invalid_argument("Error: not enough arguments.");
	std::cout << "Debug infos:" << std::endl;
	std::cout << "Channel:" << channel << std::endl;
	std::cout << "Target:" << target << std::endl;
	std::cout << "Msg:" << msg << std::endl;
	std::cout << std::endl;
	(void)cArgs.prefix;//supress
	(void)cArgs.hasPrefix;//supress
	//doKick(channel, target, msg, hasPrefix, prefix);
	return ;
}

void	Server::join(t_commandArgs & cArgs)
{
	std::map<std::string, std::string> channelPw;
	std::stack<std::string> passwords;
	std::string words;
	bool resetUserChans = false;
	int sscount = 0;
	int channelCount = 0;
	int passwordCount = 0;


	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (words == "0" && sscount != 1)
			resetUserChans = true;
		else if (resetUserChans)
			throw std::invalid_argument("Error: too many arguments.");
		else if (sscount == 0)
		{
			std::stringstream channels(words);
			std::string channelName;
			while (std::getline(channels, channelName, ','))
			{
				if (channelName[0] != '#')
					throw std::invalid_argument("Error: channel name has to start with #.");
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
				throw std::invalid_argument("Error: More passwords than channels.");
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

void	Server::invite(t_commandArgs & cArgs)
{
	std::string target;
	std::string channel;
	std::string words;
	int sscount = 0;

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
		throw std::invalid_argument("Error: not enough arguments.");
	std::cout << "Debug infos:" << std::endl;
	std::cout << "Target:" << target << std::endl;
	std::cout << "Channel:" << channel << std::endl;
	std::cout << std::endl;
	(void)cArgs.prefix;//supress
	(void)cArgs.hasPrefix;//supress
	//doInvite(target, channel, hasPrefix, prefix);
	return ;
}

#include <cerrno>
#include <cstdlib>
#include <climits>
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

// void	Server::mode(t_commandArgs & cArgs)
// {
// 	std::list<std::string> args;
// 	std::string words;
// 	std::string channel;
// 	bool activate = true;
// 	size_t requiredArgCount = 0;
// 	std::map<char, bool> modes;
// 	int sscount = 0;

// 	while (*cArgs.sstream >> words)
// 	{
// 		std::cout << "words :" << words << std::endl;
// 		if (sscount == 0)
// 		{
// 			if (words[0] != '#')
// 				throw std::invalid_argument("Error: channel name has to start with #.");
// 			channel = words;
// 		}
// 		else if (sscount == 1)
// 		{
// 			for (size_t i = 0; i < words.size(); i++)
// 			{
// 				//std::cout << "words[i] :" << words[i] << std::endl;
// 				if (words[i] == '+' && i != words.size() - 1)
// 				{
// 					if (i == 0 || (i > 0 && words[i - 1] != '+' && words[i - 1] != '-'))
// 						activate = true;
// 					else
// 						throw std::invalid_argument("Error: bad arguments.");
// 				}
// 				else if (words[i] == '-' && i != words.size() - 1)
// 				{
// 					if (i == 0 || (i > 0 && words[i - 1] != '+' && words[i - 1] != '-'))
// 						activate = false;
// 					else
// 						throw std::invalid_argument("Error: bad arguments.");
// 				}
// 				else if ((words[i] == 'i' || words[i] == 't'
// 				|| words[i] == 'k' || words[i] == 'l' || words[i] == 'o')
// 				&& modes.find(words[i]) == modes.end())
// 				{
// 					if (i != 0 && (words[i - 1] == '+' || words[i - 1] == '-'))
// 						modes[words[i]] = activate;
// 					else
// 						throw std::invalid_argument("Error: bad arguments.");
// 				}
// 				else
// 					throw std::invalid_argument("Error: bad arguments");
// 			}
// 		}
// 		else if (sscount > 1)
// 			args.push_back(words);
// 		else if (sscount > 4)
// 			throw std::invalid_argument("Error: too many arguments.");
// 		sscount++;
// 	}
// 	if (sscount < 2)
// 		throw std::invalid_argument("Error: not enough arguments.");
// 	for (std::map<char, bool>::iterator it = modes.begin(); it != modes.end(); ++it)
// 	{
// 		std::string switched;
// 		if (it->first == 'k' || it->first == 'o' || it->first == 'l')
// 		{
// 			if (it->second)
// 				requiredArgCount++;
// 		}
// 	}
// 	if (requiredArgCount != args.size())
// 		throw std::invalid_argument("Error: not enough arguments.");

// 	std::cout << "Debug infos:" << std::endl; //DO NOT SUPPRESS, PARSING IN IT !
// 	std::list<std::string>::iterator args_it = args.begin();
// 	for (std::map<char, bool>::iterator it = modes.begin(); it != modes.end(); ++it)
// 	{
// 		std::string switched;
// 		if (it->second)
// 			switched = "activated";
// 		else
// 			switched = "desactivated";
// 		std::cout << "Mode " << it->first << " has been " << switched << std::endl;
// 		if ((it->first == 'k' || it->first == 'o' || it->first == 'l') && it->second)
// 		{
// 			if (it->first == 'l' && !isInt(*args_it))	
// 				throw std::invalid_argument("Error: argument for limit is not a number.");
// 			std::cout << "Argument for " << it->first << " : " << *args_it << std::endl;
// 			args_it++;
// 		}
// 	}
// 	std::cout << std::endl;

// 	(void)cArgs.prefix;//supress
// 	(void)cArgs.hasPrefix;//supress
// 	//doMode(modes, args, hasPrefix, prefix);
// 	//

// 	std::map<std::string, Channel>::iterator currentChannelIt;
// 	currentChannelIt = _channels.find(channel);
// 	if (currentChannelIt == _channels.end())
// 		throw std::invalid_argument("Error: Channel doesn't exist.");
// 	if (!currentChannelIt->second.isOperator(*cArgs.client))
// 		throw std::invalid_argument("Error: User is not operator.");

// 	//
// 	return ;
// }


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
