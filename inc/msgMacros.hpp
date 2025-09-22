#pragma once


//>> :Armida.german-elite.net 366 nick1234 #chanchanchan :End of /NAMES list.

# define END "\r\n"

# define UINFO(nick, user) (":" + nick + "!~" + user + "@localhost")

# define JOIN(nick, user, channel) (UINFO(nick, user) + " JOIN " + channel + END)
# define RPL_NAMREPLY(nick, user, channel) (":Lisautim.serv.42network 353 " + nick + " = " + channel + " " + UINFO(nick, user) + END)
# define RPL_ENDOFNAMES(nick, channel) (":Lisautim.serv.42network 366 " + nick + " " + channel + " :End of /NAMES list." + END)
# define RPL_NOTOPIC(nick, channel) (":Lisautim.serv.42network 331 " + nick + channel + " :test." + END)
