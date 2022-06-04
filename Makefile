NAME = ircserv
INCLUDE = include/
CPP = c++ -g #-Wall -Wextra -Werror -std=c++98
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

MEBOT = mebot
B_SRCS =	main.cpp \
			Bot.cpp \
			Input.cpp \
			Symbol.cpp \
			Parser.cpp \
			Calculator.cpp
BOT_DIR = bot
BOT_SRCS = $(addprefix $(BOT_DIR)/, $(B_SRCS))
BOT_OBJ_DIR = $(OBJ_DIR)/bot
BOT_OBJS = $(addprefix $(OBJ_DIR)/, $(BOT_SRCS:.cpp=.o))

all: $(NAME)

$(OBJ_DIR):
		mkdir -p $@

$(OBJ_DIR)/%.o: %.cpp
		$(CPP) -c $< -o $@ #-I $(INCLUDE) enable later

$(NAME): $(OBJ_DIR) $(OBJS)
		$(CPP) $(OBJS) -o $(NAME)

sanitize:
	$(CPP) -o $(NAME) -fsanitize=address $(SRCS)

leaks: $(NAME)
		leaks --atExit -- ./$(NAME) 6667 1

#########################
#Make bot. Use 'mebot'
$(BOT_OBJ_DIR):
		mkdir -p $@
$(OBJ_DIR)/%.o: %.cpp
		$(CPP) -c $< -o $@
$(MEBOT): $(BOT_OBJ_DIR) $(BOT_OBJS)
		$(CPP) $(BOT_OBJS) -o $(MEBOT)
##########################

debug:
		$(CPP) -o $(NAME) -gdwarf $(SRCS)

clean:
		rm -rf $(OBJ_DIR)

fclean: clean
		rm -f $(NAME) $(MEBOT)

re: fclean all

.PHONY: all clean fclean re
