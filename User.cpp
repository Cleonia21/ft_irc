#include "User.hpp"

// User::User(string hostName, string name)
// : _hostName(hostName), _name(name)
// 	{
// 		cout << "Param constructor from User colled" << endl;
// 	}

User::User( void )
{

}

std::string User::getName( void ) const { return (_name); }
std::string User::getNick( void ) const { return (_nick); }
std::string User::getHostName( void ) const { return (_hostName); }
std::string User::getRealName( void ) const { return (_realName); }

void User::setName( std::string name ) { _name = name; }
void User::setNick( std::string nick ) { _nick = nick; }
void User::setHostName( std::string hostName ) { _hostName = hostName; }
void User::setRealName( std::string realName) { _realName = realName; }

std::ostream	&operator<<( std::ostream &ostr, User &instance )
{
	ostr	<< "\e[0;32m" << "User:" << std::endl
			<< "Name:      " << instance.getName() << std::endl
			<< "Nick:      " << instance.getNick() << std::endl
			<< "Host name: " << instance.getHostName() << std::endl
			<< "Real name: " << instance.getRealName() << std::endl << "\e[0m";
	return (ostr);
}
