#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

void nick(std::stringstream *ss, bool hasPrefix, std::string prefix){
    std::string nick;
    std::string words;
    int sscount = 0;

	while (*ss >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
            nick = words;
        else if (sscount > 0)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount != 1)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "Nick:" << nick << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doNick(nick, hasPrefix, prefix);
    return ;
}