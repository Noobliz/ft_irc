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
	doPrivateMsg(*cArgs.client, msgTarget, msg);
}

int	Server::findClient(std::string nickname)
{

	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == nickname)
			return _clients[i].getFD();
	}
	return (-2);
}

void	Server::doPrivateMsg(Client & client, std::vector<std::string> nick, std::string msg)
{
	std::map<std::string, Channel>::iterator ite2;
	int fd;
	std::string feedback;
	if (msg.empty()) // pas utile ?
	{
		feedback = NO_TEXTOSEND(client.getNickname());
		if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		return ;
	}

	for(size_t i = 0; i < nick.size(); i++)
	{
		fd = findClient(nick[i]);
		ite2 = _channels.find(nick[i]);
		if(fd >= 0)
		{
			if (_clients[fd].isAuth())
			{
				feedback = PRIVMSG(client.getNickname(), client.getUserinfo().username, _clients[fd].getNickname(), msg);
				if (send(fd, feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
				feedback = "";
			}
		}
		else if (ite2 != _channels.end())
		{
			std::cout << "J'ESSAIE D'ENVOYER UN MESSAGE DANS UN CHANNEL" << std::endl;

			std::string index = (*ite2).first;
            std::map<std::string, Client> tmp;
            tmp = _channels[index].getConnectedClients();
			if (client.isInChan(nick[i]))
			{
				std::cout << "JE SUIS BIEN DANS LE CHANNEL DONC JE PEUX" << std::endl;

				std::map<std::string, Client>::iterator ite = tmp.begin();
				for (; ite != tmp.end(); ++ite)
				{
					feedback = PRIVMSG(client.getNickname(), client.getUserinfo().username, nick[i], msg);
					if ((*ite).second.getNickname() != client.getNickname())
					{
						if (send((*ite).second.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
							throw std::runtime_error("send() failed");
					}
					feedback = "";
				}
			}
			else
			{
				feedback = ERR_CANNOTSENDTOCHAN(client.getNickname(), nick[i]);
				if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
					throw std::runtime_error("send() failed");
				feedback = "";
			}
		}
		else
		{

			feedback = ERR_NOSUCHNICK(client.getNickname(), nick[i]);
			if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
				throw std::runtime_error("send() failed");
			feedback = "";
		}
	}

}
