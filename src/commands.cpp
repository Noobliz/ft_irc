#include <commands.hpp>

void user(t_commandArgs & cArgs){
    t_userinfos userinfos;
    std::string words;
    int sscount = 0;
    std::streampos ssPos;


	while (*cArgs.sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
            userinfos.username = words;
        else if (sscount == 1)
            userinfos.hostname = words;
        else if (sscount == 2)
        {
            userinfos.servername = words;
            ssPos = cArgs.sstream->tellg();
        }
        else if (sscount == 3)
        {
            if (words[0] == ':')
            {
                userinfos.realname = (cArgs.sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
    (void)cArgs.prefix;//supress
    (void)cArgs.hasPrefix;//supress
    //doUser(userinfos, hasPrefix, prefix);
    return ;
}

void topic(t_commandArgs & cArgs)
{
    std::string channel;
    std::string msg = "";
    std::string words;
    int sscount = 0;
    std::streampos ssPos;

	while (*cArgs.sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            if (words[0] != '#')
                throw std::invalid_argument("Error: channel name has to start with #.");
            channel = words;
            ssPos = cArgs.sstream->tellg();
        }
        else if (sscount == 1)
        {
            if (words[0] == ':')
            {
                msg = (cArgs.sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
    (void)cArgs.prefix;//supress
    (void)cArgs.hasPrefix;//supress
    //doTopic(channel, msg, hasPrefix, prefix);
    return ;
}

void privmsg(t_commandArgs & cArgs)
{
    std::list<std::string> msgTarget;
    std::string msg;
    std::string words;
    int sscount = 0;
    std::streampos ssPos;



	while (*cArgs.sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            std::stringstream targets(words);
            std::string target;
            while (std::getline(targets, target, ','))
               msgTarget.push_back(target);
            ssPos = cArgs.sstream->tellg();
        }
        else if (sscount == 1)
        {
            if (words[0] == ':')
            {
                msg = (cArgs.sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
    (void)cArgs.prefix;//supress
    (void)cArgs.hasPrefix;//supress
	(void)cArgs.client; //hill
    //doPrivmsg(std::list<std::string>, msg, hasPrefix, prefix);


    return ;
}

void pass(t_commandArgs & cArgs)
{
    std::string password;
    std::string words;
    int sscount = 0;

	while (*cArgs.sstream >> words)
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

void nick(t_commandArgs & cArgs)
{
    std::string nick;
    std::string words;
    int sscount = 0;

	while (*cArgs.sstream >> words)
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
    (void)cArgs.prefix;//supress
    (void)cArgs.hasPrefix;//supress
    //doNick(nick, hasPrefix, prefix);
    return ;
}

void kick(t_commandArgs & cArgs)
{
    std::string target;
    std::string channel;
    std::string msg = "";
    std::string words;
    int sscount = 0;
    std::streampos ssPos;

	while (*cArgs.sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            if (words[0] != '#')
                throw std::invalid_argument("Error: channel name has to start with #.");
            channel = words;
        }
        else if (sscount == 1)
        {
            target = words;
            ssPos = cArgs.sstream->tellg();
        }
        else if (sscount == 2)
        {
            if (words[0] == ':')
            {
                msg = (cArgs.sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
                std::stringstream msgSStream(msg);
                std::getline(msgSStream, msg, ':');
                std::getline(msgSStream, msg);
                sscount++;
                break;
            }
            msg = words;
        }
        else if (sscount == 3)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount < 2)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "Channel:" << channel << std::endl;
    std::cout << "Target:" << target << std::endl;
    std::cout << "Msg:" << msg << std::endl;
    (void)cArgs.prefix;//supress
    (void)cArgs.hasPrefix;//supress
    //doKick(channel, target, msg, hasPrefix, prefix);
    return ;
}

void join(t_commandArgs & cArgs)
{
    std::map<std::string, std::string> channelPw;
    std::stack<std::string> passwords;
    std::string words;
    bool resetUserChans = false;
    int sscount = 0;
    int channelCount = 0;
    int passwordCount = 0;


	while (*cArgs.sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (words == "0" && sscount != 1)
            resetUserChans = true;
        else if (resetUserChans)
            throw std::invalid_argument("Error: too many arguments.");
        else if (sscount == 0)
        {
            std::stringstream channels(words);
            std::string channelName;
            while (std::getline(channels, channelName, ','))
            {
                if (channelName[0] != '#')
                    throw std::invalid_argument("Error: channel name has to start with #.");
                channelCount++;
                std::cout << "channel" << channelCount << ": " << channelName << std::endl;
                channelPw[channelName] = "";
            }
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
                throw std::invalid_argument("Error: More passwords than channels.");
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
        }
        sscount++;
    }
    if (sscount < 1)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "Map:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = channelPw.begin(); it != channelPw.end(); ++it)
    {
        std::cout << it->first << " -> " << it->second <<std::endl;
    }
    std::cout << "Reset User Channels: " << resetUserChans << std::endl;
    (void)cArgs.prefix;//supress
    (void)cArgs.hasPrefix;//supress
    //doJoin(std::map<std::string, std::string>, resetUserChans, hasPrefix, prefix);
    return ;
}

void invite(t_commandArgs & cArgs)
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
    std::cout << "Target:" << target << std::endl;
    std::cout << "Channel:" << channel << std::endl;
    (void)cArgs.prefix;//supress
    (void)cArgs.hasPrefix;//supress
    //doInvite(target, channel, hasPrefix, prefix);
    return ;
}

/*

static void user(std::stringstream *sstream, bool hasPrefix, std::string prefix){
    t_userinfos userinfos;
    std::string words;
    int sscount = 0;
    std::streampos ssPos;


	while (*sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
            userinfos.username = words;
        else if (sscount == 1)
            userinfos.hostname = words;
        else if (sscount == 2)
        {
            userinfos.servername = words;
            ssPos = sstream->tellg();
        }
        else if (sscount == 3)
        {
            if (words[0] == ':')
            {
                userinfos.realname = (sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
    std::cout << "Debug infos:" << std::endl;
    std::cout << "username:" << userinfos.username << std::endl;
    std::cout << "hostname:" << userinfos.hostname << std::endl;
    std::cout << "servername:" << userinfos.servername << std::endl;
    std::cout << "realname:" << userinfos.realname << std::endl;
    std::cout << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doUser(userinfos, hasPrefix, prefix);
    return ;
}

static void topic(std::stringstream *sstream, bool hasPrefix, std::string prefix){
    std::string channel;
    std::string msg = "";
    std::string words;
    int sscount = 0;
    std::streampos ssPos;

	while (*sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            if (words[0] != '#')
                throw std::invalid_argument("Error: channel name has to start with #.");
            channel = words;
            ssPos = sstream->tellg();
        }
        else if (sscount == 1)
        {
            if (words[0] == ':')
            {
                msg = (sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
    std::cout << "Debug infos:" << std::endl;
    std::cout << "Channel:" << channel << std::endl;
    std::cout << "New topic:" << msg << std::endl;
    std::cout << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doTopic(channel, msg, hasPrefix, prefix);
    return ;
}

static void privmsg(Client & client, std::stringstream *sstream, bool hasPrefix, std::string prefix){
    std::list<std::string> msgTarget;
    std::string msg;
    std::string words;
    int sscount = 0;
    std::streampos ssPos;



	while (*sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            std::stringstream targets(words);
            std::string target;
            while (std::getline(targets, target, ','))
               msgTarget.push_back(target);
            ssPos = sstream->tellg();
        }
        else if (sscount == 1)
        {
            if (words[0] == ':')
            {
                msg = (sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
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
    std::cout << "Debug infos:" << std::endl;
    std::cout << "List:" << std::endl;
    for (std::list<std::string>::iterator it = msgTarget.begin(); it != msgTarget.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "Msg:" << msg << std::endl;
    std::cout << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
	(void)client; //hill
    //doPrivmsg(std::list<std::string>, msg, hasPrefix, prefix);


    return ;
}

static void pass(std::stringstream *sstream){
    std::string password;
    std::string words;
    int sscount = 0;

	while (*sstream >> words)
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
    std::cout << "Debug infos:" << std::endl;
    std::cout << "Password:" << password << std::endl;
    std::cout << std::endl;
    //doPass(password, hasPrefix, prefix);
    return ;
}

static void nick(std::stringstream *sstream, bool hasPrefix, std::string prefix){
    std::string nick;
    std::string words;
    int sscount = 0;

	while (*sstream >> words)
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
    std::cout << "Debug infos:" << std::endl;
    std::cout << "Nick:" << nick << std::endl;
    std::cout << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doNick(nick, hasPrefix, prefix);
    return ;
}

static void kick(std::stringstream *sstream, bool hasPrefix, std::string prefix){
    std::string target;
    std::string channel;
    std::string msg = "";
    std::string words;
    int sscount = 0;
    std::streampos ssPos;

	while (*sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            if (words[0] != '#')
                throw std::invalid_argument("Error: channel name has to start with #.");
            channel = words;
        }
        else if (sscount == 1)
        {
            target = words;
            ssPos = sstream->tellg();
        }
        else if (sscount == 2)
        {
            if (words[0] == ':')
            {
                msg = (sstream->str()).substr(ssPos);//sstream ou *sstream ? :(
                std::stringstream msgSStream(msg);
                std::getline(msgSStream, msg, ':');
                std::getline(msgSStream, msg);
                sscount++;
                break;
            }
            msg = words;
        }
        else if (sscount == 3)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount < 2)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "Debug infos:" << std::endl;
    std::cout << "Channel:" << channel << std::endl;
    std::cout << "Target:" << target << std::endl;
    std::cout << "Msg:" << msg << std::endl;
    std::cout << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doKick(channel, target, msg, hasPrefix, prefix);
    return ;
}

static void join(std::stringstream *sstream, bool hasPrefix, std::string prefix){
    std::map<std::string, std::string> channelPw;
    std::stack<std::string> passwords;
    std::string words;
    bool resetUserChans = false;
    int sscount = 0;
    int channelCount = 0;
    int passwordCount = 0;


	while (*sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (words == "0" && sscount != 1)
            resetUserChans = true;
        else if (resetUserChans)
            throw std::invalid_argument("Error: too many arguments.");
        else if (sscount == 0)
        {
            std::stringstream channels(words);
            std::string channelName;
            while (std::getline(channels, channelName, ','))
            {
                if (channelName[0] != '#')
                    throw std::invalid_argument("Error: channel name has to start with #.");
                channelCount++;
                std::cout << "channel" << channelCount << ": " << channelName << std::endl;
                channelPw[channelName] = "";
            }
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
                throw std::invalid_argument("Error: More passwords than channels.");
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
        }
        sscount++;
    }
    if (sscount < 1)
        throw std::invalid_argument("Error: not enough arguments.");
    std::cout << "Debug infos:" << std::endl;
    std::cout << "Map:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = channelPw.begin(); it != channelPw.end(); ++it)
    {
        std::cout << it->first << " -> " << it->second <<std::endl;
    }
    std::cout << "Reset User Channels: " << resetUserChans << std::endl;
    std::cout << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doJoin(std::map<std::string, std::string>, resetUserChans, hasPrefix, prefix);
    return ;
}

static void invite(std::stringstream *sstream, bool hasPrefix, std::string prefix){
    std::string target;
    std::string channel;
    std::string words;
    int sscount = 0;

	while (*sstream >> words)
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
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doInvite(target, channel, hasPrefix, prefix);
    return ;
}

static void mode(std::stringstream *sstream, bool hasPrefix, std::string prefix)
{
    std::string newOperator;
    std::string words;
    bool activate = true;
    std::map<char, bool> modes;
    int sscount = 0;


	while (*sstream >> words)
    {
    	std::cout << "words :" << words << std::endl;
        if (sscount == 0)
        {
            for (size_t i = 0; i < words.size(); i++)
            {
                //std::cout << "words[i] :" << words[i] << std::endl;
                if (words[i] == '+' && i != words.size() - 1)
                {
                    if (i == 0 || (i > 0 && words[i - 1] != '+' && words[i - 1] != '-'))
                        activate = true;
                    else
                        throw std::invalid_argument("Error: bad arguments.");
                }
                else if (words[i] == '-' && i != words.size() - 1)
                {
                    if (i == 0 || (i > 0 && words[i - 1] != '+' && words[i - 1] != '-'))
                        activate = false;
                    else
                        throw std::invalid_argument("Error: bad arguments.");
                }
                else if ((words[i] == 'i' || words[i] == 't'
                || words[i] == 'k' || words[i] == 'l' || words[i] == 'o')
                && modes.find(words[i]) == modes.end())
                {
                    if (i != 0 && (words[i - 1] == '+' || words[i - 1] == '-'))
                        modes[words[i]] = activate;
                    else
                        throw std::invalid_argument("Error: bad arguments.");
                }
                else
                    throw std::invalid_argument("Error: bad arguments");
            }
        }
        else if (sscount == 1)
            newOperator = words;
        else if (sscount > 1)
            throw std::invalid_argument("Error: too many arguments.");
        sscount++;
    }
    if (sscount < 1)
        throw std::invalid_argument("Error: not enough arguments.");
    if (modes.find('o') != modes.end() && newOperator == "")
        throw std::invalid_argument("Error: missing target for operator change mode.");
    if (modes.find('o') == modes.end() && newOperator != "")
        throw std::invalid_argument("Error: too many arguments.");
    std::cout << "Debug infos:" << std::endl;
    for (std::map<char, bool>::iterator it = modes.begin(); it != modes.end(); ++it)
    {
        std::string switched;
        if (it->second)
            switched = "activated";
        else
            switched = "desactivated";
        if (it->first == 'o')
            std::cout << newOperator << "'s rights have been " << switched << std::endl;
        else
            std::cout << "Mode " << it->first << " has been " << switched << std::endl;
    }
    std::cout << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doMode(modes, newOperator, hasPrefix, prefix);
    return ;
}

*/
