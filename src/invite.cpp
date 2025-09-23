#include <Server.hpp>
#include <msgMacros.hpp>
#include <protocolMsg.hpp>


void	Server::invite(t_commandArgs & cArgs)
{
	std::string target;
	std::string channel;
	std::string words;
	int sscount = 0;

	while (*cArgs.sstream >> words)
	{
		std::cout << "words :" << words << std::endl;
		if (sscount == 0)
			target = words;
		else if (sscount == 1)
		{
			if (words[0] != '#')
				throw std::invalid_argument("Error: channel name has to start with #.");
			channel = words;
		}
		else if (sscount > 1)
			throw std::invalid_argument("Error: too many arguments.");
		sscount++;
	}
	if (sscount != 2)
		throw std::invalid_argument("Error: not enough arguments.");
	std::cout << "Debug infos:" << std::endl;
	std::cout << "Target:" << target << std::endl;
	std::cout << "Channel:" << channel << std::endl;
	std::cout << std::endl;
	(void)cArgs.prefix;//supress
	(void)cArgs.hasPrefix;//supress
	doInvite(*cArgs.client, target, channel);
	return ;
}


void    Server::doInvite(Client client, std::string const & target, std::string const &chan)
{
    std::map<std::string, Channel>::iterator iteChan;
    iteChan = _channels.find(chan);
    std::string feedback;
    if (iteChan == _channels.end())
    {
        feedback = ERR_NOSUCHCHANNEL(client.getNickname(), chan);
        if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
            throw std::runtime_error("send() failed");
        return ;
    }
    if (!client.isInChan(chan))
    {
        feedback = ERR_NOTONCHANNEL(client.getNickname(), chan);
        if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
            throw std::runtime_error("send() failed");
        return ;
    }

    std::map<std::string, Client> operators = (*iteChan).second.getOpperators();
    
    std::map<std::string, Client>::iterator iteSender = operators.find(client.getNickname());
    
    if ((*iteChan).second.isInInviteMode() && iteSender == operators.end())
    {
        feedback = ERR_CHANOPRIVSNEEDED(client.getNickname(), chan);
        if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
            throw std::runtime_error("send() failed");
        return ;
    }
    if (findClient(target) == -2)
    {
        feedback = ERR_NOSUCHNICK(client.getNickname(), target);
        if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
        return ;
    }
    if ((*iteChan).second.getConnectedClients().find(target) != (*iteChan).second.getConnectedClients().end())
    {
        feedback = ERR_USERONCHANNEL(client.getNickname(), target, chan);
        if (send(client.getFD(), feedback.c_str(), feedback.length(), 0) == -1)
			throw std::runtime_error("send() failed");
        return ;
    }


    std::map<int, Client>::iterator iteTarget = _clients.find(findClient(target));
    (*iteChan).second.inviteClient((*iteTarget).second);
    feedback = INVITE(client.getNickname(), client.getUserinfo().username, target, chan);
    if (send(findClient(target), feedback.c_str(), feedback.length(), 0) == -1)
		throw std::runtime_error("send() failed");

    //debug
    std::map<std::string, Client> invited = (*iteChan).second.getInvitedClient();
    if (invited.empty())
    {
        std::cout << "(none)" << std::endl;
        return;
    }

    std::map<std::string, Client>::const_iterator it = invited.begin();
    for (; it != invited.end(); ++it)
    {
        std::cout << "- " << it->second.getNickname() << std::endl;
    }
}