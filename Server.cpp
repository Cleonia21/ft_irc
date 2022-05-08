#include "Server.hpp"

Server::Server(int server_port, std::string server_password) :
	port(server_port), password(server_password)
{
	this->commands["PASS"] = &Server::pass;
	this->commands["NICK"] = &Server::nick;

	this->motd = "Welcome to IRCserv!";
}

Server::~Server(void)
{
	std::cout << "Server Destructor called" << std::endl;
}

void Server::execution(std::string input)
{
	Input new_input(input);
	User user;

	this->iter = commands.find(input); //iter - итератор контейнера с командами, ищем команду через find
	if (iter == commands.end())			//проверяем наличие команды
	{
		//error, command not found
	}
	(this->*iter->second)(user, new_input); //запускаем команду
	//(this->*commands[new_input.getCommand()])(user, new_input); //скорее всего лучше использовать верхний вариант
	//так как можно обработать ситуацию в случае с неправильно набранной командой
}

User *Server::searchUser(int key, std::string znch, std::vector<User *> users)
{
	std::string (User::*geter)( void ) const;
	if (key == SRCH_NICK)
		geter = &User::getNick;
	else if (key == SRCH_HOSTNAME)
		geter = &User::getHostName;
	else if (key == SRCH_NAME)
		geter = &User::getName;
	else
	{
		redCout("Invalid key in searchUser");
		return (NULL);//NULL
	}
	for (int i = 0; i != users.size(); i++)
	{
		if ((users[i]->*geter)() == znch)
			return (users[i]);
	}
	return (NULL);//NULL
}

