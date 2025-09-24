#include <Server.hpp>

void	Server::doTopic(t_commandArgs & cArgs, std::string & channelname, std::string & topic)
{
	(void)cArgs;
	(void)channelname;
	//? cArgs.client->isInChan(channelname)
	//!si le client n'est pas dans le channel > 442
	//!si tu trouves pas le channel > 403
	if (topic.empty())
	{
		//!si tu consultes et que y a pas > 331 RPL_NOTOPIC
		//!si tu consultes et qui y a > 332 RPL_TOPIC
	}
	else
	{
		//? attention verif si c'est en mode freemodif ou pas
		//!si tu modifies et que t'es OPE > ">> @time=2025-09-24T07:17:33.070Z :nick1234!~user1234@gaistn2v.20.unyc.it TOPIC #chan2 :super topic"
		//!si tu modifies et que t'es pas OPE > 482
	}
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
			{
				throw std::invalid_argument("Error: channel name has to start with #.");
			}
			channel = words;
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
		else if (sscount == 2)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount < 1)
	{
		throw std::invalid_argument("Error: not enough arguments.");
	}
	doTopic(cArgs, channel, msg);
}
