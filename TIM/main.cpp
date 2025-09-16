#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
void join(std::string cmd, std::stringstream *ss);

std::string getPrefix(std::stringstream *ss, bool *hasPrefix){
	std::string prefix;

	*hasPrefix = true;
	*ss >> prefix;
	if (prefix == ":")
		throw std::invalid_argument("Error: prefix cant be null");
	return prefix.substr(1);
}

void chooseCmd(std::string prefix, bool hasPrefix, std::stringstream *ss){
	std::string cmd;
	*ss >> cmd;

	(void)prefix;
	if(cmd == "PASS")
			;//
	else if(cmd == "NICK")
			;//
	else if(cmd == "USER")
			;//
	else if(cmd == "JOIN")
	{
		if (hasPrefix)
			throw std::invalid_argument("Error: invalid syntax");
		else
			join(cmd, ss);
	}
	else if(cmd == "PRIVMSG")
			;//
	else if(cmd == "KICK")
			;//
	else if(cmd == "INVITE")
			;//
	else if(cmd == "TOPIC")
			;//
	else if(cmd == "MODE")
			;//
	else
		throw std::invalid_argument("Error: unknown command");
	return ;
}


int	main(int ac, char **av)
{
	if (ac == 2)
	{
		if (av[1][0])
		{
			bool hasPrefix = false;
			std::string prefix = "";
			try
			{
				std::string str = av[1];
				std::stringstream ss(str);
				if (str[0] == ':')
				{
					prefix = getPrefix(&ss, &hasPrefix);
					std::cout << "prefix : " << prefix << std::endl;
				}
				chooseCmd(prefix, hasPrefix, &ss);
			}
			catch (std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				return 1;
			}
		}
	}
}
