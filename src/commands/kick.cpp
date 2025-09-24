#include <Server.hpp>

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

