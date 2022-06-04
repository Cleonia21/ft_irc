#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include "Input.hpp"
#include "Symbol.hpp"
#include <fstream>
#include <iomanip>
#include "Calculator.hpp"

class Bot
{
	private:
		std::string					_rawName;
		std::string					_name;
		int							_nameCount;
		std::string					_port;
		std::string					_password;
		std::queue<std::string>		_messages;
		std::vector<std::string>	_botCommand;
		std::queue<std::string>		_pendingOutMessages;
		std::string					_serverName;
		addrinfo					hints, *res;
		int							socketfd;
		std::vector<struct pollfd>	pollfds;
		int							_authenticated;
		int							_nofile;

		std::vector<Symbol> 		_abc;
		void getAbc(void);

		//socketManagement
		void socketGetaddrinfo(void);
		void socketConnect(void);
		void socketOptions(void);
		void socketBind(void);
		void socketListen(void);
		void preparePollfds(void);

		void processMessage(void);
		std::queue<std::string> split(std::string &data, char separator);
		void execute(void);
		std::vector<std::string> parsePrivmsg(std::string msg);

		//authorize
		void sendMsg(std::string &msg);

		//unused
		Bot(Bot const &src);
		Bot & operator=(Bot const & src);
		Bot(void);
	public:
		Bot(std::string port, std::string password);
		~Bot(void);

		void socketManagement(void);
		void authorize(void);
		void checkForData(void);
};
