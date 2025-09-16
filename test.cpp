#include <iostream>
#include <Client.hpp>
#include <Channel.hpp>
#include <map>

std::map<std::string, Channel>	_channels;

void	join(Client client, std::map<std::string, std::string> args)
{
	for (std::map<std::string, std::string>::iterator it = args.begin(); it != args.end(); ++it)
	{
		std::map<std::string, Channel>::iterator cit = _channels.find((*it).first);
		if (cit != _channels.end())
		{
			if (_channels[(*it).first].checkPass((*it).second))
			{
				if (_channels[(*it).first].isInvited(client)) //! peut directement verifier si le client est invité ou pas dans addClient.
					_channels[(*it).first].addClient(client);
			}
		}
		else
		{
			Channel newChan((*it).first, (*it).second);
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
