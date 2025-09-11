CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

NAME = server

SRC =   test.cpp \


OBJDIR = .obj/
OBJ = $(addprefix $(OBJDIR),$(SRC:.cpp=.o))

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@


$(OBJDIR)%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : re clean fclean all