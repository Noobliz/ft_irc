CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

NAME	= ircserv

SRC =	main.cpp					\
		src/Server.cpp				\
		src/Client.cpp				\
		src/Channel.cpp				\
		src/commands/join.cpp		\
		src/commands/pass.cpp		\
		src/commands/user.cpp		\
		src/commands/nick.cpp		\
		src/commands/topic.cpp		\
		src/commands/privmsg.cpp	\
		src/commands/invite.cpp		\
		src/commands/kick.cpp		\
		src/commands/who.cpp		\
		src/commands/mode.cpp		\
		src/commands/part.cpp		\
		src/commands/quit.cpp		\
		src/commands/bot.cpp		\

OBJDIR = .obj/
OBJ = $(addprefix $(OBJDIR),$(SRC:.cpp=.o))

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -Iinc -o $@

bot :
	make -C bot

clean :
	rm -rf $(OBJDIR)
	rm -rf bot/.obj

fclean : clean
	rm -f $(NAME)
	rm -f bot/beautyBot

re : fclean all

.PHONY : re clean fclean all bot
