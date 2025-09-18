#include <map>
#include "inc/Client.hpp"
#include <vector>
#include "inc/Channel.hpp"

// class Channel{
// 	private:

// 	public:
// 		std::vector<Client> clientsList;
// };

// std::map<int, Client> clients;

std::map<std::string, Channel> channels;

int	findClient(std::map<int, Client> clients, std::string nickname)
{
	typename std::map<int, Client>::iterator ite = clients.begin();

	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].getNickname() == nickname)
			return clients[i].getFD();
	}
	return (-2);
}
//std::vector<std::string> nick;
void	privateMsg(Client client, std::vector<std::string> nick, std::string msg)
{
	typename std::map<std::string, Channel>::iterator ite2;
	int fd;

	for(int i = 0; i < nick.size(); i++)
	{
		fd = findClient(clients, nick[i]);
		ite2 = channels.find(nick[i]);
		if(fd >= 0)
		{
			//send((fd, msg);
		}
		else if (ite2 != channels.end())
		{
			std::string index = (*ite2).first;
			ite2 = channels[index]._connectedClients.find(client.getNickname());//check if user is part of chan he want to send msg
			if (ite2 != channels[index]._connectedClients.end())
			{
				for (int i = 0; i < channels[index]._connectedClients.size(); i++)
				{
					//send(channels[index].clientsList[i].getFD(), msg);
				}
			}
		}
		else
		{
		// 	ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
        //    ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
        //    ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
        //    ERR_NOSUCHNICK
        //    RPL_AWAY
		}
	}
	
}
