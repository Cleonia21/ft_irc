NAME	=	irc.out

HPP		=	irc.hpp Channel.hpp User.hpp
CPP		=	c++ #-Wall -Wextra -Werror -std=c++98
SRCS	=	execution.cpp User.cpp parser.cpp main.cpp colorCout.cpp
OBJ		=	$(SRCS:.cpp=.o)

all		:	$(NAME)

%.o		:	%.cpp $(HPP)
			$(CPP) -c $< -o $@

$(NAME)	:	$(OBJ) MAKEFILE
			$(CPP) $(OBJ) -o $(NAME)

clean	:
			rm -f $(OBJ)

fclean	:	clean
			rm -f $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
