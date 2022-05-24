#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include "User.hpp"
#include "Channel.hpp"
#include "Input.hpp"
#include "irc.hpp"

#define SERVER_DISCONNECT -1

class Server
{
	private:
		typedef int (Server::*Command)(User &, Input &);

		std::string									_port;
		std::string									_password;
		std::vector<User *>							_users;
		std::vector<Channel *>						_channels;
		std::map<std::string, Command>				_commands;
		std::map<std::string, Command>::iterator 	_iter; //internal iterator for commands
		std::string									_motd;

		int											socketfd;
		addrinfo									hints, *res;
		std::vector<struct pollfd>					pollfds;

		//utility functions
		bool isNickValid(std::string &nick) const;
		bool containsNickname(const std::string &nick) const;
		int checkForRegistration(User &user);

		//socketManagement
		void socketGetaddrinfo(void);
		void socketCreate(void);
		void socketOptions(void);
		void socketBind(void);
		void socketListen(void);
		void preparePollfds(void);

		//commands
		int pass(User &user, Input &input);
		int nick(User &user, Input &input);
		int user(User &user, Input &input);


		//unused
		Server(void);
		Server(Server const &src);
		Server & operator=(Server const & src);
		User *searchUser(int key, std::string znch);
	public:
		Server(std::string server_port, std::string server_password);
		~Server(void);
		void socketManagement(void);
		void acceptConnection(void);
		void disconnectUsers(void);

		void execution(User &user);
};
