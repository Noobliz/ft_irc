#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

void topic(std::stringstream *ss, bool hasPrefix, std::string prefix){
    std::string channel;
    std::string msg = "";
    std::string words;
    int sscount = 0;
    std::streampos ssPos;

	while (*ss >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            if (words[0] != '#')
                throw std::invalid_argument("Error: channel name has to start with #.");
            channel = words;
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
        else if (sscount == 2)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount < 1)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "Channel:" << channel << std::endl;
    std::cout << "New topic:" << msg << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doTopic(channel, msg, hasPrefix, prefix);
    return ;
}