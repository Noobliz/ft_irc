#include "bot.hpp"

Bot::Bot(int port, const std::string &pass): _port(port), _password(pass), _nickname("beautyBot")
{
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_port = htons(_port);
    _servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr convert string in binary
    _sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockFd == -1)
		throw std::invalid_argument("Error: socket fail");
}

void Bot::sendMsg(const std::string &msg)const
{
    std::string full_msg = msg + "\r\n";
    if (send(_sockFd, full_msg.c_str(), full_msg.length(), 0) == -1) //le bot parle au serveur au travers de son socket
        throw std::invalid_argument("Error: send fail");
}

void    Bot::connectToServ()
{
    socklen_t	addrlen = sizeof(_servAddr);
    //connecte le tuyau du bot au serveur pour ouvrir la communication
    if (connect(_sockFd, (struct sockaddr *)&_servAddr, addrlen) == -1)//replace accept
        throw std::invalid_argument("Error: connect fail");
    
    //send les messages d'auth
    sendMsg("NICK " + _nickname);
    sendMsg("USER " + _nickname + " 0 * :beautyBot");
    sendMsg("PASS " + _password);


}

void    Bot::run()
{
    std::string	concatstr = "";
    bool    connected = true;
    while (connected)
    {
        char buffer[2] = "";
        size_t r = recv(_sockFd, buffer, 1, 0);
        if (r == 0)
        {
            std::cout << "server fermax" << std::endl;
            close(_sockFd);
            connected = false;
        }
        else if (r > 0)
        {
            if (buffer[0] != '\n')
            {
                concatstr += buffer;
                //std::cout << concatstr << std::endl;
            }
            else
            {
                concatstr += "\n";
                //repartitor(_clients[_events[i].data.fd], concatstr);
                concatstr = "";
            }
        }
        else
        {
            throw std::invalid_argument("Error: recv failed");
        }
    }

}