#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
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

enum Disconnect_user
{
	SERVER_DISCONNECT = -2,
	SERVER_LOSTCONNECT = 0,
	SERVER_QUIT = 1,
	SERVER_KILL = 2
};

class Server
{
	private:
		typedef int (Server::*Command)(User &, Input &);

		std::string									_port;
		std::string									_password;
		std::vector<User *>							_users;
		std::map<std::string, std::string>			_operators;
		std::map<std::string, Channel *>			_channels;
		std::map<std::string, Command>				_commands;
		std::map<std::string, Command>::iterator 	_iter; //internal iterator for commands
		std::vector<std::string>					_motd;

		int											socketfd;
		addrinfo									hints, *res;
		std::vector<struct pollfd>					pollfds;

		//utility functions
		void execution(User &user);
		void disconnectUser(User &user, int reason);
		bool isNickValid(std::string &nick) const;
		bool containsNickname(const std::string &nick) const;
		bool containsChannel(const std::string &channel) const;
		int checkForRegistration(User &user);
		void sendWelcome(const User &user) const;
		void sendMOTD(const User &user) const;
		int sendPM(User &user, Input &input, int silent);
		std::string getTrueNickname(std::string &mask);

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
        int join(User &user, Input &input);
        int kick(User &user, Input &input);
		int mode(User &user, Input &input);
		int privmsg(User &user, Input &input);
		int notice(User &user, Input &input);
        int topic(User &user, Input &input);
        int part(User &user, Input &input);
		int oper(User &user, Input &input);
		int kill(User &user, Input &input);
        int names(User &user, Input &input);
        int list(User &user, Input &input);
        int quit(User &user, Input &input);

		bool printer(Input &input);


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
		void clearEmptyChannels(void);

};
