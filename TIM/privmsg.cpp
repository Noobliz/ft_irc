#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <list>

void privmsg(std::stringstream *ss, bool hasPrefix, std::string prefix){
    std::list<std::string> msgTarget;
    std::string msg;
    std::string words;
    int sscount = 0;
    std::streampos ssPos;


	while (*ss >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            std::stringstream targets(words);
            std::string target;
            while (std::getline(targets, target, ','))
               msgTarget.push_back(target);
            ssPos = ss->tellg();
        }
        else if (sscount == 1)
        {
            if (words[0] == ':')
            {
                msg = (ss->str()).substr(ssPos);//ss ou *ss ? :(
                std::stringstream msgSStream(msg);
                std::getline(msgSStream, msg, ':');
                std::getline(msgSStream, msg);
                sscount++;
                break;
            }
            msg = words;
        }
        else if(sscount > 1)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount != 2)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "List:" << std::endl;
    for (std::list<std::string>::iterator it = msgTarget.begin(); it != msgTarget.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "Msg:" << msg << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doPrivmsg(std::list<std::string>, msg, hasPrefix, prefix);
    return ;
}