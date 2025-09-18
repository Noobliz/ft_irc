#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <list>

typedef struct  s_userinfos{
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
}           t_userinfos;

void user(std::stringstream *ss, bool hasPrefix, std::string prefix){
    t_userinfos userinfos;
    std::string words;
    int sscount = 0;
    std::streampos ssPos;


	while (*ss >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
            userinfos.username = words;
        else if (sscount == 1)
            userinfos.hostname = words;
        else if (sscount == 2)
        {
            userinfos.servername = words;
            ssPos = ss->tellg();
        }
        else if (sscount == 3)
        {
            if (words[0] == ':')
            {
                userinfos.realname = (ss->str()).substr(ssPos);//ss ou *ss ? :(
                std::stringstream msgSStream(userinfos.realname);
                std::getline(msgSStream, userinfos.realname, ':');
                std::getline(msgSStream, userinfos.realname);
                sscount++;
                break;
            }
            userinfos.realname = words;
        }
        else if(sscount > 3)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount != 4)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "username:" << userinfos.username << std::endl;
    std::cout << "hostname:" << userinfos.hostname << std::endl;
    std::cout << "servername:" << userinfos.servername << std::endl;
    std::cout << "realname:" << userinfos.realname << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doPrivmsg(userinfos, hasPrefix, prefix);
    return ;
}