#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

void pass(std::stringstream *ss){
    std::string password;
    std::string words;
    int sscount = 0;

	while (*ss >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
            password = words;
        else if (sscount > 0)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount != 1)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "Password:" << password << std::endl;
    //doPass(password, hasPrefix, prefix);
    return ;
}