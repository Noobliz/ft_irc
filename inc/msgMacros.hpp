#pragma once

# define END "\r\n"

# define UINFO(nick, user) (nick + "!~" + user + "@localhost")

//? Feedback d'auth
# define ERR_PASSWDMISMATCH ":Lisautim 464 :Password incorrect\r\n"
# define ERR_ALREADYREGISTERED ":Lisautim 462 :You may not reregister\r\n"

//? Feedback de Nick
# define NICK(oldnick, newnick) (":" + oldnick + " NICK " + newnick + END)
# define ERR_NONICKNAMEGIVEN ":Lisautim 431 :No nickname given\r\n"
# define ERR_NICKNAMEINUSE(nick) (":Lisautim 433 " + nick + " :Nickname is already in use" + END)
# define ERR_ERRONEUSNICKNAME(nick) (":Lisautim 433 " + nick + " :Erroneus nickname" + END)

//? Feedback de Join
# define JOIN(nick, user, channel) (":" + UINFO(nick, user) + " JOIN " + channel + END)
# define RPL_NAMREPLY(nick, channel) (":Lisautim 353 " + nick + " = " + channel + " :")
# define RPL_ENDOFNAMES(nick, channel) (":Lisautim 366 " + nick + " " + channel + " :End of /NAMES list." + END)
# define RPL_NOTOPIC(nick, channel) (":Lisautim 331 " + nick + " " + channel + " :No topic is set" + END)
# define RPL_TOPIC(nick, channel, topic) (":Lisautim 332 " + nick + " " + channel + " :" + topic + END)

# define ERR_BADCHANNELKEY(nick, channel) (":Lisautim 475 " + nick + " " + channel + " :Cannot join channel (+k)" + END)
# define ERR_CHANNELISFULL(nick, channel) (":Lisautim 471 " + nick + " " + channel + " :Cannot join channel (+l)" + END)
# define ERR_INVITEONLYCHAN(nick, channel) (":Lisautim 473 " + nick + " " + channel + " :Cannot join channel (+i)" + END)

//? Utile pour hexchat ?
# define ERR_TOOMANYCHANNELS(nick, channel) (":Lisautim 405 " + nick + " " + channel + " :You have joined too many channels" + END)

//? Utile pour mode / privmsg ?
# define ERR_BADCHANMASK(nick, channel) (":Lisautim 476 " + nick + " " + channel + " :Bad Channel Mask" + END)

//? Erreur de parsing
# define ERR_NEEDMOREPARAMS(nick, command) (":Lisautim 461 " + nick + " " + command + " :Not enough parameters" + END)
