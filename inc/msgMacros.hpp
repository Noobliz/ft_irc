#pragma once


//>> :Armida.german-elite.net 366 nick1234 #chanchanchan :End of /NAMES list.

# define END "\r\n"

# define UINFO(nick, user) (nick + "!~" + user + "@localhost")

# define JOIN(nick, user, channel) (":" + UINFO(nick, user) + " JOIN " + channel + END)
# define RPL_NAMREPLY(nick, channel) (":Lisautim 353 " + nick + " = " + channel + " :")
# define RPL_ENDOFNAMES(nick, channel) (":Lisautim 366 " + nick + " " + channel + " :End of /NAMES list." + END)
# define RPL_NOTOPIC(nick, channel) (":Lisautim 331 " + nick + " " + channel + " :No topic is set" + END)
# define RPL_TOPIC(nick, channel, topic) (":Lisautim 332 " + nick + " " + channel + " :" + topic + END)
