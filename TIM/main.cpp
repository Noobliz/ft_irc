#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
void join(std::stringstream *ss, bool hasPrefix, std::string prefix);
void privmsg(std::stringstream *ss, bool hasPrefix, std::string prefix);
void pass(std::stringstream *ss);
void nick(std::stringstream *ss, bool hasPrefix, std::string prefix);
void user(std::stringstream *ss, bool hasPrefix, std::string prefix);
void invite(std::stringstream *ss, bool hasPrefix, std::string prefix);
void kick(std::stringstream *ss, bool hasPrefix, std::string prefix);
void topic(std::stringstream *ss, bool hasPrefix, std::string prefix);

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
	{
		if (hasPrefix)
			throw std::invalid_argument("Error: PASS cant have a prefix.");
		pass(ss);
	}
	else if(cmd == "NICK")
		nick(ss, hasPrefix, prefix);
	else if(cmd == "USER")
		user(ss, hasPrefix, prefix);
	else if(cmd == "JOIN")
		join(ss, hasPrefix, prefix);
	else if(cmd == "PRIVMSG")
		privmsg(ss, hasPrefix, prefix);
	else if(cmd == "KICK")
		kick(ss, hasPrefix, prefix);
	else if(cmd == "INVITE")
		invite(ss, hasPrefix, prefix);
	else if(cmd == "TOPIC")
		topic(ss, hasPrefix, prefix);
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
