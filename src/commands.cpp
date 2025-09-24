#include <Server.hpp>





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

void	Server::mode(t_commandArgs & cArgs)
{
	std::string newOperator;
	std::string words;
	bool activate = true;
	std::map<char, bool> modes;
	int sscount = 0;


	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
		{
			for (size_t i = 0; i < words.size(); i++)
			{
				//std::cout << "words[i] :" << words[i] << std::endl;
				if (words[i] == '+' && i != words.size() - 1)
				{
					if (i == 0 || (i > 0 && words[i - 1] != '+' && words[i - 1] != '-'))
						activate = true;
					else
						throw std::invalid_argument("Error: bad arguments.");
				}
				else if (words[i] == '-' && i != words.size() - 1)
				{
					if (i == 0 || (i > 0 && words[i - 1] != '+' && words[i - 1] != '-'))
						activate = false;
					else
						throw std::invalid_argument("Error: bad arguments.");
				}
				else if ((words[i] == 'i' || words[i] == 't'
				|| words[i] == 'k' || words[i] == 'l' || words[i] == 'o')
				&& modes.find(words[i]) == modes.end())
				{
					if (i != 0 && (words[i - 1] == '+' || words[i - 1] == '-'))
						modes[words[i]] = activate;
					else
						throw std::invalid_argument("Error: bad arguments.");
				}
				else
					throw std::invalid_argument("Error: bad arguments");
			}
		}
		else if (sscount == 1)
			newOperator = words;
		else if (sscount > 1)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount < 1)
		throw std::invalid_argument("Error: not enough arguments.");
	if (modes.find('o') != modes.end() && newOperator == "")
		throw std::invalid_argument("Error: missing target for operator change mode.");
	if (modes.find('o') == modes.end() && newOperator != "")
		throw std::invalid_argument("Error: too many arguments.");
	std::cout << "Debug infos:" << std::endl;
	for (std::map<char, bool>::iterator it = modes.begin(); it != modes.end(); ++it)
	{
		std::string switched;
		if (it->second)
			switched = "activated";
		else
			switched = "desactivated";
		if (it->first == 'o')
			std::cout << newOperator << "'s rights have been " << switched << std::endl;
		else
			std::cout << "Mode " << it->first << " has been " << switched << std::endl;
	}
	std::cout << std::endl;
	(void)cArgs.prefix;//supress
	(void)cArgs.hasPrefix;//supress
	//doMode(modes, newOperator, hasPrefix, prefix);
	return ;
}
