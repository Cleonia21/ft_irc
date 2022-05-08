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

		int					port;
		std::string			password;
		std::vector<User *>	users;
		std::vector<Channel *>	channels;
		std::map<std::string, Command>	commands;
		std::map<std::string, Command>::iterator iter;
		std::string			motd;

		//commands
		int pass(User &user, Input &input);
		int nick(User &user, Input &input);


		//unused
		Server(void);
		Server(Server const &src);
		Server & operator=(Server const & src);
	public:
		Server(int server_port, std::string server_password);
		~Server(void);

		User *searchUser(int key, std::string znch, std::vector<User*> users);
		void execution(std::string input);
};


