#include "Server.hpp"

Server::Server(int server_port, std::string server_password) :
	_port(server_port), _password(server_password)
{
	_commands["PASS"] = &Server::pass;
	_commands["NICK"] = &Server::nick;

	_motd = "Welcome to IRCserv!";
}

Server::~Server(void)
{
	// std::cout << "Server Destructor called" << std::endl;
}

void Server::execution(std::string input)
{
	Input newInput(input);
	User user;

	_iter = _commands.find(newInput.getCommand());
	if (_iter == _commands.end()) //проверяем наличие команды
		std::cout << "error, command not found(execution)" << std::endl;
	else
		(this->*_iter->second)(user, newInput); //запускаем команду
}

User *Server::searchUser(int key, std::string znch)
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
		return (NULL);
	}
	for (int i = 0; i != _users.size(); i++)
	{
		if ((_users[i]->*geter)() == znch)
			return (_users[i]);
	}
	return (NULL);
}

