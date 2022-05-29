#include "Server.hpp"
#include <sys/socket.h>

Server::Server(std::string server_port, std::string server_password) :
	_port(server_port), _password(server_password)
{
	_commands["PASS"] = &Server::pass;
	_commands["NICK"] = &Server::nick;
	_commands["USER"] = &Server::user;
	_commands["MODE"] = &Server::mode;
	_commands["PRIVMSG"] = &Server::privmsg;
	_commands["NOTICE"] = &Server::notice;
    _commands["JOIN"] = &Server::join;
    _commands["KICK"] = &Server::kick;
	_commands["TOPIC"] = &Server::topic;
    _commands["PART"] = &Server::part;
	_commands["NAMES"] = &Server::names;
	_commands["LIST"] = &Server::list;
	_commands["QUIT"] = &Server::quit;
	_motd.push_back("Welcome to IRCserv!");
	_motd.push_back("Be good!");
	memset(&hints, 0, sizeof(hints)); //making sure addrinfo is empty
}

Server::~Server(void)
{
	for (size_t i = 0; i < this->_users.size(); i++)
	{
		close(_users[i]->getSocketfd());
		delete this->_users[i];
	}
	std::map<std::string, Channel *>::iterator it = _channels.begin();
	std::map<std::string, Channel *>::iterator end = _channels.end();
	for (; it != end; it++)
		delete (*it).second;
	close(socketfd);
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
				if ((_users[i - 1]->getFlags() & USER_DISCONNECTED))
					disconnectUser(*_users[i - 1]);
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

		if (this->printer(cmd))
			continue;

		std::cout << "User IP: " << user.getHostName() << " Socket: " << user.getSocketfd() << std::endl
			<< cmd << std::endl; //log of message
		_iter = _commands.find(cmd.getCommand());
		if (!(user.getFlags() & USER_REGISTERED) &&
				cmd.getCommand() != "QUIT" && cmd.getCommand() != "PASS" &&
				cmd.getCommand() != "USER" && cmd.getCommand() != "NICK")
			sendServerReply(user, ERR_NOTREGISTERED);
		else
		{
			if (_iter == _commands.end()) //проверяем наличие команды
				sendServerReply(user, ERR_UNKNOWNCOMMAND, cmd.getCommand());
			else
			{
				int ret = (this->*_iter->second)(user, cmd); //запускаем команду
				if (ret == SERVER_DISCONNECT)
				{
					user.setFlags(USER_DISCONNECTED);
					disconnectUser(user);
					return ;
				}
			}
		}
	}
}

void Server::disconnectUser(User &user)
{
		std::cout << "Disconnect! Socket: " << user.getSocketfd() << std::endl;
		//
		//Need channel notices and removals
		//
		close(user.getSocketfd());
		std::vector<User *>::iterator it = std::find(_users.begin(), _users.end(), &user);
		delete *it;
		size_t i = std::distance(_users.begin(), it);
		_users.erase(it);
		pollfds.erase(pollfds.begin() + i + 1);
}

void Server::clearEmptyChannels(void)
{
	std::map<std::string, Channel *>::iterator it = _channels.begin();
	std::map<std::string, Channel *>::iterator end = _channels.end();
	for (; it != end;)
	{
		if ((*it).second->isEmpty())
		{
			delete (*it).second;
			_channels.erase(it);
			it = _channels.begin();
		}
		else
			it++;
	}
}

bool Server::isNickValid(std::string &nick) const
{
	if (nick.length() > 9)
		return (false);
	std::string specials = "-\\`^[]{}";
	for (size_t i = 0; i < nick.size(); i++)
	{
		if (!((nick[i] >= 'a' && nick[i] <= 'z')
		|| (nick[i] >= 'A' && nick[i] <= 'Z')
		|| (nick[i] >= '0' && nick[i] <= '9')
		|| (specials.find(nick[i]) != std::string::npos)))
			return (false);
	}
	return (true);
}

bool Server::containsNickname(const std::string &nick) const
{
	size_t count = _users.size();
	for (size_t i = 0; i < count; i++)
		if (_users[i]->getNick() == nick)
			return (true);
	return (false);
}

bool Server::containsChannel(const std::string &channel) const
{
	try
	{
		_channels.at(channel);
		return (true);
	}
	catch (std::exception &e) {}
	return (false);
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

bool Server::printer(Input &input)
{
	if (input.getCommand() != "su")
		return (false);
	if (input.getParams().empty())
		std::cout << "Use su users/channels" << std::endl;
	else if (input.getParams()[0] == "users")
	{
		std::cout << "My users:" << std::endl;
		std::vector<User *>::iterator users = _users.begin();
		for ( ; users != _users.end(); users++)
			std::cout << **users << std::endl;
	}
	else if (input.getParams()[0] == "channels")
	{
		std::cout << "My channels" << std::endl;
		std::map<std::string, Channel *>::iterator chanels = _channels.begin();
		for ( ; chanels != _channels.end(); chanels++)
			std::cout << *((*chanels).second) << std::endl;
	}
	else
		std::cout << "Use su users/channels" << std::endl;
	return (true);
}
