#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

void invite(std::stringstream *ss, bool hasPrefix, std::string prefix){
    std::string target;
    std::string channel;
    std::string words;
    int sscount = 0;

	while (*ss >> words)
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
    std::cout << "Target:" << target << std::endl;
    std::cout << "Channel:" << channel << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doInvite(target, channel, hasPrefix, prefix);
    return ;
}