#pragma once

#include "irc.hpp"

class User
{
private:
	string	_name;
	string	_nick;
	string	_hostName;
	string	_realName;
	string	_password;

public:
	User( void );

	void setName( string );
	void setNick( string );
	void setHostName( string );
	void setRealName( string );
	string getName( void );
	string getNick( void );
	string getHostName( void );
	string getRealName( void );
	// void set( void );
	// void get( void );
	// void set( void );
	// void get( void );
};
