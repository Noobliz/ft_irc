CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

NAME = ircserv

SRC =	main.cpp				\
		src/Server.cpp			\
		src/commands.cpp		\
		src/Client.cpp			\
		src/Channel.cpp			\
		src/commands/join.cpp	\

OBJDIR = .obj/
OBJ = $(addprefix $(OBJDIR),$(SRC:.cpp=.o))

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -Iinc -o $@

clean :
	rm -rf $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : re clean fclean all
