#include "Server.hpp"
#include <sys/socket.h>

Server::Server(std::string server_port, std::string server_password) :
	_port(server_port), _password(server_password)
{
	_commands["PASS"] = &Server::pass;
	_commands["NICK"] = &Server::nick;
//    _commands["USER"] = &Server::user;
//    _commands["PRIVMSG"] = &Server::privmsg;
//    _commands["NOTICE"] = &Server::notice;
//    _commands["JOIN"] = &Server::join;
//    _commands["KICK"] = &Server::kick;

	_motd = "Welcome to IRCserv!";
	memset(&hints, 0, sizeof(hints)); //making sure addrinfo is empty
}

Server::~Server(void)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		delete this->_users[i];
	}
}

/*
 * Сокет для коннекта
*/

void Server::socketManagement(void)
{
	this->socketGetaddrinfo();
	this->socketCreate();
	this->socketOptions();
	this->socketBind();
	freeaddrinfo(res);
	this->socketListen();
	this->preparePollfds();
}

void Server::socketGetaddrinfo(void)
{
	int status;
	this->hints.ai_family = AF_INET;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, _port.c_str(), &hints, &res)) != 0)
	{
		std::cout << "Socket getaddinfo failed. Error: ";
		gai_strerror(status);
		std::cout << ", Errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	
/*	void *addr;
	char ipstr[INET6_ADDRSTRLEN];
	addrinfo *p;
	for (p = res; p != NULL; p = p->ai_next)
	{
		sockaddr_in *ipv4 = (sockaddr_in *)res->ai_addr;
		addr = &(ipv4->sin_addr);
		inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
		std::cout << ipstr << std::endl;
	}
*/
}

void Server::socketCreate(void)
{
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketfd == -1)
	{
		std::perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	//Seting socket to Non-blocking mode
	fcntl(socketfd, F_SETFL, O_NONBLOCK);
}

// Basically needed for port to not hang after disconnect
void Server::socketOptions(void)
{
	int optval = 1; //flag for setsockopt, any non-zero value is needed for flags to be set.
	if (setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0)
	{
		std::perror("Socket setsockopt failed");
		exit(EXIT_FAILURE);
	}
}

void Server::socketBind(void)
{
	if (bind(this->socketfd, res->ai_addr, res->ai_addrlen) != 0)
	{
		std::perror("Socket bind failed");
		exit(EXIT_FAILURE);
	}
}

void Server::socketListen(void)
{
	if (listen(socketfd, 10) != 0)
	{
		std::perror("Socket listen failed");
		exit(EXIT_FAILURE);
	}
}

void Server::preparePollfds(void)
{
	struct pollfd listener;
	listener.fd = socketfd;
	listener.events = POLLIN;
	listener.revents = 0;
	pollfds.push_back(listener);
}

/*
 * Функции сокета закочились
*/

void Server::acceptConnection(void)
{
	sockaddr_in addr;
	socklen_t addrlen;
	char remoteIP[INET_ADDRSTRLEN];
	int new_fd; //new connection

	int poll_count = poll(pollfds.data(), pollfds.size(), 1);
	if (poll_count == 0)
		return ;
	if (poll_count == -1)
	{
		std::perror("Poll failed");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < pollfds.size(); i++)
	{
		//Check if sockets are ready to read
		if (pollfds[i].revents & POLLIN) //Got incoming data
		{
			//Check if listener socket got new connection
			if (pollfds[i].fd == socketfd) //Handle that new connection
			{
				addrlen = sizeof(addr);
				new_fd = accept(socketfd, (struct sockaddr *)&addr, &addrlen);
				if (new_fd == -1)
					std::perror("Socket accept failed");
				else
				{
					inet_ntop(AF_INET, &(addr.sin_addr), remoteIP, INET_ADDRSTRLEN);
					struct pollfd new_pfd;
					new_pfd.fd = new_fd;
					new_pfd.events = POLLIN;
					new_pfd.revents = 0;
					pollfds.push_back(new_pfd);
					_users.push_back(new User(remoteIP, new_fd));
					std::cout << "New connection! Socket: " << new_fd << std::endl;
				}
			}
			else //Handle incoming message
			{
				_users[i - 1]->processMessage();
				if (!(_users[i - 1]->getFlags() & USER_DISCONNECTED))
					execution(*(_users[i - 1]));
			}
			//pollfds[i].revents = 0;
		}
	}
}

void Server::execution(User &user)
{
	//check if any messages AND if message ends on new line (if not -> incomlete -> do not process)
	while (user.getMessages().size() != 0 &&
			user.getMessages().front()[user.getMessages().front().size() - 1] == '\n')
	{
		Input cmd(user.getNextMessage());
		std::cout << "User IP: " << user.getHostName() << " Socket: " << user.getSocketfd() << std::endl
			<< cmd << std::endl; //log of message
		_iter = _commands.find(cmd.getCommand());
		if (!(user.getFlags() & USER_REGISTERED) &&
				cmd.getCommand() != "QUIT" && cmd.getCommand() != "PASS" &&
				cmd.getCommand() != "USER" && cmd.getCommand() != "NICK")
		//	send(user.getSocketfd(), "Not registered\n", 15, 0); //temporary disabled to not get spammed
			;
		else
		{
			if (_iter == _commands.end()) //проверяем наличие команды
				send(user.getSocketfd(), "Error, command not found\n", 25, 0);
			else
				(this->*_iter->second)(*this->_users[0], cmd); //запускаем команду
		}
	}
}

void Server::disconnectUsers(void)
{
	for (int i = 0; i < _users.size(); i++)
	{
		if (!(_users[i]->getFlags() & USER_DISCONNECTED))
			continue;
		std::cout << "Disconnect! Socket: " << _users[i]->getSocketfd() << std::endl;
		close(_users[i]->getSocketfd());
		//
		//Need channel notices and removals
		//
		delete _users[i];
		_users.erase(_users.begin() + i);
		pollfds.erase(pollfds.begin() + i + 1);
		i--;
	}
}

User *Server::searchUser(int key, std::string znch)
{
	std::string (User::*geter)( void ) const;
	if (key == SRCH_NICK)
		geter = &User::getNick;
	else if (key == SRCH_HOSTNAME)
		geter = &User::getHostName;
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

