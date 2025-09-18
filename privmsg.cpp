#include <map>
#include "inc/Client.hpp"
#include <vector>

class Channel{
	private:

	public:
		std::vector<Client> clientsList;
};

std::map<std::string, Client> clients;

std::map<std::string, Channel> channels;


//std::vector<std::string> nick;
void	privateMsg(Client client, std::vector<std::string> nick, std::string msg)
{
	typename std::map<std::string, Client>::iterator ite;
	typename std::map<std::string, Channel>::iterator ite2;

	for(int i = 0; i < nick.size(); i++)
	{
		ite = clients.find(nick[i]);
		ite2 = channels.find(nick[i]);
		if(ite != clients.end())
		{
			//send((*ite).fd, msg);
		}
		else if (ite2 != channels.end())// NEED TO CHECK IF CLIENT HAS ACCESS TO SERV
		{
			std::string index = (*ite2).first;
			for (int i = 0; i < (*ite2).second.clientsList.size() / channels[index].clientsList.size(); i++)
			{
				send(channels[index].clientsList[i].getFD(), msg);
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

int main ()
{
	return 0;
}