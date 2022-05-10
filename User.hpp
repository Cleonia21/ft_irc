#pragma once

#include <string>
#include <iomanip>
#include "Input.hpp"

class User
{
private:
	std::string	_name;
	std::string	_nick;
	std::string	_hostName;
	std::string	_realName;
	std::string	_password;

public:
	User( void );

	void setName( std::string );
	void setNick( std::string );
	void setHostName( std::string );
	void setRealName( std::string );
	std::string getName( void ) const;
	std::string getNick( void ) const;
	std::string getHostName( void ) const;
	std::string getRealName( void ) const;
	// void set( void );
	// void get( void );
	// void set( void );
	// void get( void );
};

std::ostream	&operator<<( std::ostream &ostr, User &instance );
