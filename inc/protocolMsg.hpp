#pragma once

# define SERV() std::string \
(":Lisautim")

# define UINFO(nick, user) \
(nick + "!~" + user + "@localhost")

# define END "\r\n"

/**/

//? Feedback d'auth
# define ERR_PASSWDMISMATCH							":Lisautim 464 :Password incorrect\r\n"
# define ERR_ALREADYREGISTERED						":Lisautim 462 :You may not reregister\r\n"
# define ERR_NOTREGISTERED							":Lisautim 451 :You have not registered\r\n"

//? Feedback de Nick
# define NICK(oldnick, newnick)						(":" + oldnick + " NICK " + newnick + END)
# define ERR_NONICKNAMEGIVEN						":Lisautim 431 :No nickname given\r\n"
# define ERR_NICKNAMEINUSE(nick)					(SERV() + " 433 " + nick + " :Nickname is already in use" + END)
# define ERR_ERRONEUSNICKNAME(nick)					(SERV() + " 432 " + nick + " :Erroneus nickname" + END)
# define ERR_ERRONEUSNICKNAME(nick)					(SERV() + " 432 " + nick + " :Erroneus nickname" + END)

//? Feedback de Join
# define JOIN(nick, user, channel)					(":" + UINFO(nick, user) + " JOIN " + channel + END)
# define RPL_NAMREPLY(nick, channel)				(SERV() + " 353 " + nick + " = " + channel + " :")
# define RPL_ENDOFNAMES(nick, channel)				(SERV() + " 366 " + nick + " " + channel + " :End of /NAMES list." + END)

# define RPL_WHOREPLY(nick, user, target, chan)		(SERV() + " 352 " + nick + " " + chan + " ~" + user + " localhost * " + target + " H" + END)
# define RPL_WHOREPLY_OPE(nick, user, target, chan)	(SERV() + " 352 " + nick + " " + chan + " ~" + user + " localhost * " + target + " H@" + END)
# define RPL_ENDOFWHO(nick, chan)					(SERV() + " 315 " + nick + " " + chan + " :End of /WHO list." + END)

//? Feedback de Topic
# define TOPIC(nick, user, channel, msg)			(":" + UINFO(nick, user) + " TOPIC " + channel + " :" + msg + END)
# define RPL_NOTOPIC(nick, channel)					(SERV() + " 331 " + nick + " " + channel + " :No topic is set" + END)
# define RPL_TOPIC(nick, channel, topic)			(SERV() + " 332 " + nick + " " + channel + " :" + topic + END)

# define ERR_BADCHANNELKEY(nick, channel)	(SERV() + " 475 " + nick + " " + channel + " :Cannot join channel (+k)" + END)
# define ERR_CHANNELISFULL(nick, channel)	(SERV() + " 471 " + nick + " " + channel + " :Cannot join channel (+l)" + END)
# define ERR_INVITEONLYCHAN(nick, channel)	(SERV() + " 473 " + nick + " " + channel + " :Cannot join channel (+i)" + END)

//? Utile pour hexchat ?
# define ERR_TOOMANYCHANNELS(nick, channel)	(SERV() + " 405 " + nick + " " + channel + " :You have joined too many channels" + END)

//? Utile pour mode / privmsg ?
# define RPL_CHANNELMODEIS(nick, channel, modestring, mode_arguments) \
(SERV() + " 324 " + nick + " " + channel + " " + modestring + " " + mode_arguments + END)
# define ERR_USERNOTINCHANNEL(nick, target, channel)	(SERV() + " 441 " + nick + " " + target + " " + channel + " :is not on channel" + END)

# define ERR_UNKNOWNMODE(nick, modechar)	(SERV() + " 472 " + nick + " " + modechar + " :is unknown mode char to me" + END)
# define ERR_BADCHANMASK(nick, channel)		(SERV() + " 476 " + nick + " " + channel + " :Bad Channel Mask" + END)
# define RPL_MODECHANGED1(channel, mode)\
(SERV() + " MODE " + channel + " " + mode + END)
# define RPL_MODECHANGED2(channel, mode, arg)\
(SERV() + " MODE " + channel + " " + mode + " " + arg + END)

//? Erreur de parsing
# define ERR_NEEDMOREPARAMS(nick, command)	(SERV() + " 461 " + nick + " " + command + " :Not enough parameters" + END)

# define PART(nick, user, chan, msg)		(":" + UINFO(nick, user) + " PART " + chan + " :" + msg + END)
# define QUIT(nick, user, msg)				(":" + UINFO(nick, user) + " QUIT :" + msg + END)

//:server <code numérique> <nick> <cible> :<explication>

# define PRIVMSG(nick, user, target, msg) \
(":" + UINFO(nick, user) + " PRIVMSG " + target + " " + msg + END)

# define INVITE(nick, user, target, chan) \
(":" + UINFO(nick, user) + " INVITE " + target + " " + chan + END)

# define KICK(nick, user, chan, target, msg) \
(":" + UINFO(nick, user) + " KICK " + chan + " " + target + " " + msg + END)

# define ERR_NOSUCHNICK(nick, target) \
(SERV() + " 401 " + nick + " " + target + " :No such nick/channel" + END)

# define ERR_CANNOTSENDTOCHAN(nick, channel) \
(SERV() + " 404 " + nick + " " + channel + " :Cannot send to channel" + END)

//no_recipient

# define NO_TEXTOSEND(nick) \
(SERV() + " 412 " + nick + " :No text to send" + END)

# define ERR_NOSUCHCHANNEL(nick, channel) \
(SERV() + " 403 " + nick + " " + channel + " :No such channel" + END)

# define ERR_NOTONCHANNEL(nick, channel) \
(SERV() + " " + nick + " " + channel + " :You're not on that channel" + END)

# define ERR_CHANOPRIVSNEEDED(nick, channel) \
(SERV() + " 482 " + nick + " " + channel + " :You're not channel operator" + END)

# define ERR_USERONCHANNEL(nick, target, channel) \
(SERV() + " 443 " + nick + " " + target + " " + channel + " :is already on channel" + END)

//?Welcome feedback

# define RPL_WELCOME(nick)	("Lisautim :Welcome to the localhost Network, " + nick + END)
# define RPL_YOURHOST		"Lisautim :Your host is Lisautim, running version 0.000001\r\n"
# define RPL_CREATED		"Lisautim :This server was created 8 years ago\r\n"
# define RPL_MYINFO			"Lisautim Lisautim 0.000001 - +kiopl\r\n"
# define RPL_ISUPPORT		"Lisautim PREFIX=@ CHANTYPES=# NICKLEN=9 :are supported by this server\r\n"
# define WELCOME(nick)		(RPL_WELCOME(nick) + RPL_YOURHOST + RPL_CREATED + RPL_MYINFO + RPL_ISUPPORT)
