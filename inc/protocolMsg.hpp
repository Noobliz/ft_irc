#pragma once

# define SERV() std::string \
(":Lisautim")

# define UINFO(nick, user) \
(nick + "!~" + user + "@localhost")

# define END "\r\n"

/**/

//? Feedback d'auth
# define ERR_PASSWDMISMATCH					":Lisautim 464 :Password incorrect\r\n"
# define ERR_ALREADYREGISTERED				":Lisautim 462 :You may not reregister\r\n"

//? Feedback de Nick
# define NICK(oldnick, newnick)				(":" + oldnick + " NICK " + newnick + END)
# define ERR_NONICKNAMEGIVEN				":Lisautim 431 :No nickname given\r\n"
# define ERR_NICKNAMEINUSE(nick)			(SERV() + " 433 " + nick + " :Nickname is already in use" + END)
# define ERR_ERRONEUSNICKNAME(nick)			(SERV() + " 433 " + nick + " :Erroneus nickname" + END)

//? Feedback de Join
# define JOIN(nick, user, channel)			(":" + UINFO(nick, user) + " JOIN " + channel + END)
# define RPL_NAMREPLY(nick, channel)		(SERV() + " 353 " + nick + " = " + channel + " :")
# define RPL_ENDOFNAMES(nick, channel)		(SERV() + " 366 " + nick + " " + channel + " :End of /NAMES list." + END)
# define RPL_NOTOPIC(nick, channel)			(SERV() + " 331 " + nick + " " + channel + " :No topic is set" + END)
# define RPL_TOPIC(nick, channel, topic)	(SERV() + " 332 " + nick + " " + channel + " :" + topic + END)

# define ERR_BADCHANNELKEY(nick, channel)	(SERV() + " 475 " + nick + " " + channel + " :Cannot join channel (+k)" + END)
# define ERR_CHANNELISFULL(nick, channel)	(SERV() + " 471 " + nick + " " + channel + " :Cannot join channel (+l)" + END)
# define ERR_INVITEONLYCHAN(nick, channel)	(SERV() + " 473 " + nick + " " + channel + " :Cannot join channel (+i)" + END)

//? Utile pour hexchat ?
# define ERR_TOOMANYCHANNELS(nick, channel)	(SERV() + " 405 " + nick + " " + channel + " :You have joined too many channels" + END)

//? Utile pour mode / privmsg ?
# define ERR_BADCHANMASK(nick, channel)		(SERV() + " 476 " + nick + " " + channel + " :Bad Channel Mask" + END)

//? Erreur de parsing
# define ERR_NEEDMOREPARAMS(nick, command)	(SERV() + " 461 " + nick + " " + command + " :Not enough parameters" + END)

//:server <code numérique> <nick> <cible> :<explication>

# define PRIVMSG(nick, user, target, msg) \
(":" + UINFO(nick, user) + " PRIVMSG " + target + " " + msg + END)

# define INVITE(nick, user, target, chan) \
(":" + UINFO(nick, user) + " INVITE " + target + " " + chan + END)

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
(SERV() + " 442 " + nick + " " + channel + " :You're not on that channel" + END)

# define ERR_CHANOPRIVSNEEDED(nick, channel) \
(SERV() + " 482 " + nick + " " + channel + " :You're not channel operator" + END)

# define ERR_USERONCHANNEL(nick, target, channel) \
(SERV() + " 443 " + nick + " " + target + " " + channel + " :is already on channel" + END)
