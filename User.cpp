#include "User.hpp"

// User::User(string hostName, string name)
// : _hostName(hostName), _name(name)
// 	{
// 		cout << "Param constructor from User colled" << endl;
// 	}

User::User( void )
{
	cout << "Default constructor from User colled" << endl;
}

string User::getName( void ) { return (_name); }
string User::getNick( void ) { return (_nick); }
string User::getHostName( void ) { return (_hostName); }
string User::getRealName( void ) { return (_realName); }

void User::setName( string name ) { _name = name; }
void User::setNick( string nick ) { _nick = nick; }
void User::setHostName( string hostName ) { _hostName = hostName; }
void User::setRealName( string realName) { _realName = realName; }
