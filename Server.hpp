#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "User.hpp"
#include "Channel.hpp"
#include "Input.hpp"
#include "irc.hpp"


class Server
{
	private:
		typedef int (Server::*Command)(User &, Input &);

		int											_port;
		std::string									_password;
		std::vector<User *>							_users;
		std::vector<Channel *>						_channels;
		std::map<std::string, Command>				_commands;
		std::map<std::string, Command>::iterator 	_iter;
		std::string									_motd;

		//commands
		int pass(User &user, Input &input);
		int nick(User &user, Input &input);


		//unused
		Server(void);
		Server(Server const &src);
		Server & operator=(Server const & src);
		User *searchUser(int key, std::string znch);
	public:
		Server(int server_port, std::string server_password);
		~Server(void);

		void execution(std::string input);
};
