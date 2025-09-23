#include <Server.hpp>

void	Server::pass(t_commandArgs & cArgs)
{
	std::string	password;
	std::string	words;
	std::string	err_feedback;
	int			sscount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
			password = words;
		else if (sscount > 0)
		{
			throw std::invalid_argument("Error: too many arguments.");
		}
		sscount++;
	}
	if (sscount != 1)
	{
		err_feedback = ERR_NEEDMOREPARAMS(cArgs.client->getNickname(), "PASS");
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: not enough arguments.");
	}
	if (_password == password)
	{
		if (!cArgs.client->isAuth())
			cArgs.client->setPassValid(true);
		else
		{
			err_feedback = ERR_ALREADYREGISTERED;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		}
	}
	else
	{
		err_feedback = ERR_PASSWDMISMATCH;
		if (send(cArgs.client->getFD(), err_feedback.c_str(), err_feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
		throw std::invalid_argument("Error: bad password.");
	}
}
