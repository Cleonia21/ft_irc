NAME	=	ircserv

INCLUDE =	include/
CPP		=	c++ #-Wall -Wextra -Werror -std=c++98
SRCS	=	Server.cpp \
			Server_commands.cpp \
			User.cpp \
			Channel.cpp \
			Input.cpp \
			main.cpp \
			colorCout.cpp
OBJ		=	$(SRCS:.cpp=.o)

all		:	$(NAME)

%.o		:	%.cpp
			$(CPP) -c $< -o $@ #-I $(INCLUDE) enable later

$(NAME)	:	$(OBJ) MAKEFILE
			$(CPP) $(OBJ) -o $(NAME)

clean	:
			rm -f $(OBJ)

fclean	:	clean
			rm -f $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
