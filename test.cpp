#include <iostream>
#include <Client.hpp>
#include <Channel.hpp>
#include <map>

std::map<std::string, Channel>	_channels;

//? attention, quand un client join il doit voir tous les messages du channel
void	join(Client client, std::map<std::string, std::string> args)
{
	for (std::map<std::string, std::string>::iterator it = args.begin(); it != args.end(); ++it)
	{
		std::map<std::string, Channel>::iterator cit = _channels.find((*it).first);
		if (cit != _channels.end())
		{
			if (_channels[(*it).first].checkPassword((*it).second))
			{
				//? attention, addClient ne rajoutera pas de client s'il est pas invité et que le server est en inviteMode
				_channels[(*it).first].addClient(client);
			}
		}
		else
		{
			Channel newChan(client, (*it).first, (*it).second);
			_channels[(*it).first] = newChan;
		}
	}
	/* accusé de réception ? */
}

//? test
int	main()
{
	std::map<int, std::string> test;

	test[5] = "coucou";
	test[9] = "salut";
	test[5] = "bonjour";

	return 0;
}
