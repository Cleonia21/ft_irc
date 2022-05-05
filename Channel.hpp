#pragma once

class User;

#include <list>
#include "User.hpp"

class Channel
{
	list<const User *>	_users;
	list<const User *>	_operators;
public:
	Channel( void );

	void addUser(const User &user);
	void dellUser(const User &user);
};
