

#pragma once

#include <msgMacros.hpp>

//:server <code numérique> <nick> <cible> :<explication>

#define PRIVMSG(nick, user, target, msg) \
(UINFO(nick, user) + " PRIVMSG " + target + " " + msg + END)

#define INVITE(nick, user, target, chan) \
(UINFO(nick, user) + " INVITE " + target + " " + chan + END)

#define SERV() std::string \
(":Lisautim.serv.42network")

#define ERR_NOSUCHNICK(nick, target) \
(SERV() + " 401 " + nick + " " + target + " :No such nick/channel" + END)

#define ERR_CANNOTSENDTOCHAN(nick, channel) \
(SERV() + " 404 " + nick + " " + channel + " :Cannot send to channel" + END)

//no_recipient

#define NO_TEXTOSEND(nick) \
(SERV() + " 412 " + nick + " :No text to send" + END)

#define ERR_NOSUCHCHANNEL(nick, channel) \
(SERV() + " 403 " + nick + " " + channel + " :No such channel" + END)

#define ERR_NOTONCHANNEL(nick, channel) \
(SERV() + " 442 " + nick + " " + channel + " :You're not on that channel" + END)

#define ERR_CHANOPRIVSNEEDED(nick, channel) \
(SERV() + " 482 " + nick + " " + channel + " :You're not channel operator" + END)

#define ERR_USERONCHANNEL(nick, target, channel) \
(SERV() + " 443 " + nick + " " + target + " " + channel + " :is already on channel" + END)

