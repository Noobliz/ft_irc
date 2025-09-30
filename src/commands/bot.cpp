#include <Server.hpp>


void    Server::bot(t_commandArgs & cArgs)
{
    int fd = findClient("beautyBot");
    std::map<int, Client>::iterator iteBot = _clients.find(fd);
    std::string cmds = *cArgs.sstream;
    if (iteBot != _clients.end())
        sendMsg("activation " + (cArgs).client->getNickname(), fd);
}