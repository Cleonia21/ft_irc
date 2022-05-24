NAME = ircserv
INCLUDE = include/
CPP = c++ #-Wall -Wextra -Werror -std=c++98
SRCS =		Server.cpp \
			Server_commands.cpp \
			User.cpp \
			Channel.cpp \
			Input.cpp \
			main.cpp \
			colorCout.cpp \
			sendServerReply.cpp
OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(OBJ_DIR):
		mkdir -p $@

$(OBJ_DIR)/%.o: %.cpp
		$(CPP) -c $< -o $@ #-I $(INCLUDE) enable later

$(NAME): $(OBJ_DIR) $(OBJS)
		$(CPP) $(OBJS) -o $(NAME)

clean:
		rm -rf $(OBJ_DIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
