

#pragma once

#include <msgMacros.hpp>

//:server <code numérique> <nick> <cible> :<explication>

#define PRIVMSG(nick, user, target, msg) \
(UINFO(nick, user) + " PRIVMSG " + target + " :" + msg + END)

#define SERV() std::string \
(":Lisautim.serv.42network")

#define ERR_NOSUCHNICK(nick, target) \
(SERV() + " 401 " + nick + " " + target + " :No such nick/channel" + END)

#define ERR_CANNOTSENDTOCHAN(nick, channel) \
(SERV() + " 404 " + nick + " " + channel + " :Cannot send to channel" + END)

//no_recipient
//no_texttosend