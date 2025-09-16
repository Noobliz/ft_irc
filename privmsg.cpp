#include <map>
#include "inc/Client.hpp"
#include <vector>


std::map<std::string, Client> clients;

std::map<std::string, Channel> channels;


//std::vector<std::string> nick;
void	privateMsg(std::vector<std::string> nick, std::string msg)
{
	typename std::map<std::string, Client>::iterator ite;
	typename std::map<std::string, Client>::iterator ite2;

	for(int i = 0; i < nick.size(); i++)
	{
		ite = clients.find(nick[i]);
		ite2 = channels.find(nick[i]);
		if(ite != clients.end())
		{
			//send((*ite).fd, msg);
		}
		else if (ite2 != channels.end())
		{
			for (int i = 0; i < (*ite2).clientList.size(), i++)
			{
				send(clientList[i].getFd(), msg);
			}
		}
		else
		{
			//Error : chan or client not found
		}
	}
	
}

int main ()
{
	return 0;
}