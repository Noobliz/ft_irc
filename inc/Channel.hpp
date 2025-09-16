#pragma once

#include <vector>

class Channel
{
	public:

		Channel(std::string name, std::string pass);

		bool checkPass(std::string pass);
		void addClient(Client client);
		bool isInvited(Client client) { if (_inviteMode == false) return true;}

	private:
	
		bool _inviteMode;
		std::vector<Client> connectedClients;
		std::vector<Client> invitedClients;

};
