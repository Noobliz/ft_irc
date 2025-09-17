#include <iostream>
#include <sstream>
#include <map>
#include <stack>

void join(std::string cmd, std::stringstream *ss){
    std::map<std::string, std::string> channelPw;
    std::stack<std::string> passwords;
    std::string words;
    bool resetUserChans = false;
    int sscount = 0;
    int channelCount = 0;
    int passwordCount = 0;
    std::cout << "COMMAND :" << cmd << std::endl;


	while (*ss >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (words == "0" && sscount != 2)
            resetUserChans = true;    
        else if (resetUserChans)
            throw std::invalid_argument("Error: too many arguments");
        else if (sscount == 0)
        {
            std::stringstream channels(words);
            std::string channelName;
            while (std::getline(channels, channelName, ','))
            {
                if (channelName[0] != '#')
                    throw std::invalid_argument("Error: channel name has to start with #");
                channelCount++;
                std::cout << "channel" << channelCount << ": " << channelName << std::endl;
                channelPw[channelName] = "";
            }
            sscount++;
        }
        else if (sscount == 1)
        {
            std::stringstream pwList(words);
            std::string password;
            while (std::getline(pwList, password, ','))
            {
                passwordCount++;
                std::cout << "password" << passwordCount << ": " << password << std::endl;
                passwords.push(password);
            }
            if (passwordCount > channelCount)
                throw std::invalid_argument("Error: More passwords than channels");
            std::map<std::string, std::string>::reverse_iterator it = channelPw.rbegin();
            while (channelCount > passwordCount)
            {
                it++;
                channelCount--;
            }
            while (it != channelPw.rend())
            {
                it->second = passwords.top();
                passwords.pop();
                it++;
            }
            sscount++;
        }
    }
    std::cout << "Map:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = channelPw.begin(); it != channelPw.end(); ++it)
    {
        std::cout << it->first << " -> " << it->second <<std::endl;
    }
    std::cout << "Reset User Channels: " << resetUserChans << std::endl;
    //doJoin(std::map<std::string, std::string>, resetUserChans);
    return ;
}