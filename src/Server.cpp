#include <Server.hpp>

Server::Server(uint16_t const & port, std::string & password) : _port(port), _password(password) {}

Server::~Server()
{
	close(_sockfd);
	close(_epollfd);
}

int	Server::findClient(std::string nickname)
{
	typename std::map<int, Client>::iterator ite = _clients.begin();

	for (int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == nickname)
			return _clients[i].getFD();
	}
	return (-2);
}

void	Server::privateMsg(Client client, std::vector<std::string> nick, std::string msg)
{
	typename std::map<std::string, Channel>::iterator ite2;
	int fd;

	for(int i = 0; i < nick.size(); i++)
	{
		fd = findClient(nick[i]);
		ite2 = _channels.find(nick[i]);
		if(fd >= 0)
		{
			send(fd, msg.c_str(), msg.size(), 0);
		}
		else if (ite2 != _channels.end())
		{
			std::string index = (*ite2).first;
			if (client.isInChan(nick[i]))
			{
				
				for (int i = 0; i < _channels[index].getConnectedClients().size(); i++)
				{
					//send(channels[index].clientsList[i].getFD(), msg);
				}
			}
		}
		else
		{
		// 	ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
        //    ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
        //    ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
        //    ERR_NOSUCHNICK
        //    RPL_AWAY
		}
	}
	
}


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
    std::cout << "username:" << userinfos.username << std::endl;
    std::cout << "hostname:" << userinfos.hostname << std::endl;
    std::cout << "servername:" << userinfos.servername << std::endl;
    std::cout << "realname:" << userinfos.realname << std::endl;
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
    std::cout << "Channel:" << channel << std::endl;
    std::cout << "New topic:" << msg << std::endl;
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
    std::cout << "List:" << std::endl;
    for (std::list<std::string>::iterator it = msgTarget.begin(); it != msgTarget.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "Msg:" << msg << std::endl;
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
    std::cout << "Password:" << password << std::endl;
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
    std::cout << "Nick:" << nick << std::endl;
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
    std::cout << "Channel:" << channel << std::endl;
    std::cout << "Target:" << target << std::endl;
    std::cout << "Msg:" << msg << std::endl;
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
    std::cout << "Map:" << std::endl;
    for (std::map<std::string, std::string>::iterator it = channelPw.begin(); it != channelPw.end(); ++it)
    {
        std::cout << it->first << " -> " << it->second <<std::endl;
    }
    std::cout << "Reset User Channels: " << resetUserChans << std::endl;
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
    std::cout << "Target:" << target << std::endl;
    std::cout << "Channel:" << channel << std::endl;
    (void)prefix;//supress
    (void)hasPrefix;//supress
    //doInvite(target, channel, hasPrefix, prefix);
    return ;
}

static std::string getPrefix(std::stringstream *sstream, bool *hasPrefix)
{
	std::string prefix;

	*hasPrefix = true;
	*sstream >> prefix;
	if (prefix == ":")
		throw std::invalid_argument("Error: prefix cant be null");
	return prefix.substr(1);
}

static void chooseCmd(Client & client, std::string prefix, bool hasPrefix, std::stringstream *sstream)
{
	std::string cmd;
	*sstream >> cmd;

	(void)prefix;
	if(cmd == "PASS")
	{
		if (hasPrefix)
			throw std::invalid_argument("Error: PASS cant have a prefix.");
		pass(sstream);
	}
	else if(cmd == "NICK")
		nick(sstream, hasPrefix, prefix);
	else if(cmd == "USER")
		user(sstream, hasPrefix, prefix);
	else if(cmd == "JOIN")
		join(sstream, hasPrefix, prefix);
	else if(cmd == "PRIVMSG")
		privmsg(client, sstream, hasPrefix, prefix);
	else if(cmd == "KICK")
		kick(sstream, hasPrefix, prefix);
	else if(cmd == "INVITE")
		invite(sstream, hasPrefix, prefix);
	else if(cmd == "TOPIC")
		topic(sstream, hasPrefix, prefix);
	else if(cmd == "MODE")
			;//
	else
		throw std::invalid_argument("Error: unknown command");
	return ;
}

int	Server::repartitor(Client & client, std::string str)
{
	std::cout << "string reçue dans repartitor : " << str << std::endl;

	if (!str.empty())
	{
		bool hasPrefix = false;
		std::string prefix = "";
		try
		{
			std::stringstream sstream(str);
			if (str[0] == ':')
			{
				prefix = getPrefix(&sstream, &hasPrefix);
				std::cout << "prefix : " << prefix << std::endl;
			}
			chooseCmd(client, prefix, hasPrefix, &sstream);
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	return 0;
}

void	Server::init(void)
{
	int opt = 1;

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(_port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd == -1)
		throw std::invalid_argument("Error: socket fail");

	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::invalid_argument("Error: fcntl fail");

	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::invalid_argument("Error: setsockopt fail");

	if (bind(_sockfd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
		throw std::invalid_argument("Error: bind fail");

	if (listen(_sockfd, MAX_CLIENT) == -1)
		throw std::invalid_argument("Error: listen fail");

	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
		throw std::invalid_argument("Error: epoll_create1 fail");
}

void	Server::run(void)
{
	socklen_t	addrlen = sizeof(_addr);
	std::string	concatstr = "";

	_ev.data.fd = _sockfd;
	_ev.events = EPOLLIN;

	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &_ev) == -1)
		throw std::invalid_argument("Error: epoll_ctl fail");

	while (1)
	{
		int	n = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
		if (n == -1)
			throw std::invalid_argument("Error: epoll_wait fail");

		for (int i = 0; i < n; i++)
		{
			if (_events[i].data.fd == _sockfd)
			{
				int			tmpfd;

				tmpfd = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);
				if (tmpfd == -1)
					throw std::invalid_argument("Error: accept fail");
				//client.setFD(tmpfd);

				std::cout << "j'ai ecoute un client" << std::endl;

				_ev.events = EPOLLIN;
				//_ev.data.fd = client.getFD();
				_ev.data.fd = tmpfd;

				Client client(tmpfd);
				_clients[tmpfd] = client;

				// if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, client.getFD(), &_ev) == -1)
				if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, tmpfd, &_ev) == -1)
					throw std::invalid_argument("Error: epoll_ctl fail");

			}
			else
			{
				if (_events[i].events & EPOLLIN)
				{
					//?detecter la commande. "JOIN #chan1" "PRIVMSG #chan1 :bonjour a tou.te.s"

					// un buffer pour recevoir des trucs
					// passer ce qu'on a recu (sans doute faudra t il reconstruire le buffer a la maniere d'un gnl (tkt))
					// utiliser la fonction timRepartitor() avec le buffer reconstruit
					// puis dans timRepartitor, mettre nos fonctions a la suite du parsingTim()

					char buffer[1024] = "";
					size_t r = recv(_events[i].data.fd, buffer, 1024, 0);
					if (r == 0)
					{
						std::cout << "client fermax" << std::endl;
						close(_events[i].data.fd);
					}
					else if (r > 0)
					{
						std::cout << "je recois un message" << std::endl;
						if (buffer[0] != 0)
						{
							concatstr = buffer;
							std::cout << concatstr.size() << std::endl;
							repartitor(_clients[_events[i].data.fd], concatstr);
							concatstr = "";
						}

					}
					else
						throw std::invalid_argument("Error: recv fail");

					// size_t s = send(_events[i].data.fd, (void *)"j'ai bien recu ton message merci\n\0", 35, 0);

					// if (s == 0)
					// {
					// 	std::cout << "propre" << std::endl;
					// }
					// else if (s > 0)
					// {
					// 	std::cout << "retour ok" << std::endl;
					// }
					// else
					// {
					// 	//ici on peut modifier les events pour unlock EPOLLOUT et donc attendre que le buffer soit OK apparemment
					// }

				}

				if (_events[i].events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR))
				{
					std::cout << "the client number " << _events[i].data.fd << " has been disconnected." << std::endl;
				}
			}
		}
	}
}
