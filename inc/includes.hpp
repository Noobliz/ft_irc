#pragma once

#include <iostream>
#include <sstream>

typedef struct s_userinfos
{
	std::string	username;
	std::string	hostname;
	std::string	servername;
	std::string	realname;
}	t_userinfos;

typedef struct s_commandArgs
{
	Client				&client;
	std::string			prefix;
	bool				hasPrefix;
	std::stringstream	*sstream;
}	t_commandArgs;
