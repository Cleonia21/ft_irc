#pragma once

#include <vector>
#include <string>
#include "User.hpp"

class Channel
{
	private:
		std::string					name;
		std::vector<const User *>	users;
		std::vector<const User *>	operators;


		Channel( void );
	public:
		Channel(std::string channel_name);

		void addUser(const User &user);
		void dellUser(const User &user);
};
