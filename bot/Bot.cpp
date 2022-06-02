#include "Bot.hpp"

Bot::Bot(std::string port, std::string password) 
	:_port(port), _password(password)
{
	_name = "mebot";
	_rawName = "mebot";
	_nameCount = 0;
	_authenticated = 0;
	memset(&hints, 0, sizeof(hints)); //making sure addrinfo is empty
	this->getAbc();
}

void Bot::getAbc(void)
{
	std::vector<std::string> symbol;
	std::string	line;

	symbol.push_back("bad symbol");
	_abc.resize(128, Symbol(symbol));
	symbol.clear();

	std::ifstream	fin("bot/ABC.bot");
	if (!fin)
		std::cout << "error in get Abc" << std::endl;
	
	unsigned char c = 0;
	while (std::getline(fin, line))
	{
		if (line[0] == '-')
		{
			_abc[c] = Symbol(symbol);
			symbol.clear();
			c = line[1];
			std::cout << c << std::endl;
		}
		else
			symbol.push_back(line);
	}	
}

Bot::~Bot(void)
{
}

void Bot::socketManagement(void)
{
	this->socketGetaddrinfo();
	this->socketConnect();
	freeaddrinfo(res);
	this->preparePollfds();
}

void Bot::socketGetaddrinfo(void)
{
	int status;
	this->hints.ai_family = AF_INET;
	this->hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo("localhost", _port.c_str(), &hints, &res)) != 0)
	{
		std::cout << "Socket getaddinfo failed. Error: ";
		gai_strerror(status);
		std::cout << ", Errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Bot::socketConnect(void)
{
	addrinfo *rp;
	for (rp = res; rp != NULL; rp = rp->ai_next)
	{
		socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketfd == -1)
			continue;
		if (connect(socketfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break; //Success
		close(socketfd);
	}
	if (rp == NULL)
	{
		std::perror("Could not connect");
		exit(EXIT_FAILURE);
	}
	//Seting socket to Non-blocking mode
	fcntl(socketfd, F_SETFL, O_NONBLOCK);
}

void Bot::preparePollfds(void)
{
	struct pollfd client;
	client.fd = socketfd;
	client.events = POLLIN;
	client.revents = 0;
	pollfds.push_back(client);
}

void Bot::authorize(void)
{
	std::string pass = "PASS " + _password + "\n";
	std::string nick = "NICK " + _name + "\n";
	std::string user = "USER bot * * bot\n";

	sendMsg(pass);
	sendMsg(nick);
	sendMsg(user);

}

void Bot::checkForData(void)
{
	int poll_count = poll(pollfds.data(), pollfds.size(), 1);
	if (poll_count == 0)
		return ;
	if (poll_count == -1)
	{
		std::perror("Poll failed");
		exit(EXIT_FAILURE);
	}
	//Check if socket is ready to read
	if (pollfds[0].revents & POLLIN) //Got incoming data
	{
		processMessage();
		execute();
	}
	else if (pollfds[0].revents & POLLOUT) //Write to server
	{
		if (!_authenticated)
		{
			std::string nick = "NICK " + _name + "\n";
			sendMsg(nick);
		}
		while (_pendingOutMessages.size())
		{
			sendMsg(_pendingOutMessages.front());
			_pendingOutMessages.pop();
		}
		pollfds[0].events &= ~POLLOUT;
	}
}

void Bot::processMessage(void)
{
	std::string data;
	char buffer[256];
	int nbytes;

	if (_messages.size() != 0) //Grab remaining data from last recv call
	{
		std::cout << "check in" << std::endl;
		data = _messages.front();
	}
	while ((nbytes = recv(socketfd, buffer, sizeof(buffer), 0)) > 0)
	{
		buffer[nbytes] = 0;
		data += buffer;
		if (data.find('\n') != std::string::npos) //if found new line
			break;
	}
	//	if (data.length() > 512)
	//		data = data.substr(0, 510) + "\r\n";
	if (nbytes == 0)
	{
		std::cout << "Disconnected!" << std::endl;
		exit(EXIT_FAILURE);
	}
	while (data.find("\r\n") != std::string::npos)
		data.replace(data.find("\r\n"), 2, "\n");
	if (data.size() > 1)
		_messages = split(data, '\n');
}

std::queue<std::string> Bot::split(std::string &data, char separator)
{
	std::queue<std::string> new_messages;
	std::string::iterator i = data.begin();
	while (i != data.end())
	{
		while (i != data.end() && *i == separator) //walk through separator
			i++;
		if (i == data.end())
			break ;
		std::string::iterator j = std::find(i, data.end(), separator); //find separator in data
		if (j != data.end()) //push if separator found, including separator
			new_messages.push(std::string(i, j + 1));
		else //still push even if no separator (incomplete message) Is it needed?
			new_messages.push(std::string(i, j));
		i = j;
	}
	return new_messages;
}


void Bot::execute(void)
{
	//check if any messages AND if message ends on new line (if not -> incomlete -> do not process)
	while (_messages.size() != 0 &&
			_messages.front()[_messages.front().size() - 1] == '\n')
	{
		std::string msg(_messages.front().begin(), _messages.front().end() - 1);
		Input cmd(msg);
		_messages.pop();

		std::cout << cmd << std::endl; //log of message
		if (!_authenticated) //not fully autorized
		{
			if (cmd.getCommand() == "376")
			{
				_authenticated = 1;
				_serverName = cmd.getPrefix();
				std::cout << "Authenticated!" << std::endl;
			}
			else if (cmd.getCommand() == "433")
			{
				std::stringstream ss;
				ss << _nameCount;
				_name = _rawName + ss.str();
				_nameCount++;
				_nameCount = _nameCount % 10000;
				pollfds[0].events |= POLLOUT;
			}
		}
		else //authenticated and ready to receive commands
		{
			if (cmd.getCommand() == "PRIVMSG")
			{
				std::string msg;
				std::vector<std::string> command = parsePrivmsg(cmd.getParams()[1]);
				if (command.size() != 2)
				{
					msg = "NOTICE " + cmd.getPrefix() + " :Usage <shape> <char>\n";
					_pendingOutMessages.push(msg);
					msg = "NOTICE " + cmd.getPrefix() + " :shapes: <DIAMOND>\n";
					_pendingOutMessages.push(msg);
				}
				else if (command[0] == "PRINT")
				{
					std::vector<std::string> symbol;
					unsigned char c;

					for (int i = 0; command[1][i]; i++)
					{
						c = command[1][i];
						std::cout << c << std::endl;
						symbol = _abc[c].getSymbol();
						for (std::vector<std::string>::iterator j = symbol.begin(); j != symbol.end(); ++j)
						{
							msg = "NOTICE " + cmd.getPrefix() + " :  " + (*j) + " \n";
							_pendingOutMessages.push(msg);
						}
						msg = "NOTICE " + cmd.getPrefix() + " :  " + " " + " \t\n";
						_pendingOutMessages.push(msg);
					}
				}
				else if (command[0] == "DIAMOND")
				{
					char c = command[1][0];
					msg = "NOTICE " + cmd.getPrefix() + " :  " + c + "  \n";
					_pendingOutMessages.push(msg);
					msg = "NOTICE " + cmd.getPrefix() + " : " + c + " " + c + " \n";
					_pendingOutMessages.push(msg);
					msg = "NOTICE " + cmd.getPrefix() + " :" + c + "   " + c + "\n";
					_pendingOutMessages.push(msg);
					msg = "NOTICE " + cmd.getPrefix() + " : " + c + " " + c + " \n";
					_pendingOutMessages.push(msg);
					msg = "NOTICE " + cmd.getPrefix() + " :  " + c + "  \n";
					_pendingOutMessages.push(msg);
				}
				else
				{
					msg = "NOTICE " + cmd.getPrefix() + " :shapes: <DIAMOND>\n";
					_pendingOutMessages.push(msg);
				}
				pollfds[0].events |= POLLOUT;
			}
		}
	}
}

std::vector<std::string> Bot::parsePrivmsg(std::string msg)
{
	std::cout << msg << std::endl;
	std::vector<std::string> command;
	int i = 0;
	while (msg.size())
	{
		for (; i < msg.length(); i++)
			if (msg[i] == ' ')
				break ;
		command.push_back(msg.substr(0, i));
		for (; i < msg.length(); i++)
			if (msg[i] != ' ')
				break ;
		msg.erase(0, i);
	}
	return (command);
}

void Bot::sendMsg(std::string &msg)
{
	if (send(socketfd, msg.c_str(), msg.size(), 0) < 0)
		std::cerr << "Cannot send data: " << msg << std::endl;
}
