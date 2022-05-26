#include "User.hpp"

User::User(std::string hostName, int new_fd)
	: _hostName(hostName), _socketfd(new_fd)
{
	_flags = 0;
}

User::~User( void )
{

}

void User::processMessage(void)
{
	std::string data;
	char buffer[256];
	int nbytes;

	if (messages.size() != 0) //Grab remaining data from last recv call
	{
		std::cout << "check in" << std::endl;
		data = messages.front();
	}
	while ((nbytes = recv(_socketfd, buffer, sizeof(buffer), 0)) > 0)
	{
		buffer[nbytes] = 0;
		data += buffer;
		if (data.find('\n') != std::string::npos) //if found new line
			break;
	}
//	if (data.length() > 512)
//		data = data.substr(0, 510) + "\r\n";
	if (nbytes == 0)
		setFlags(USER_DISCONNECTED);
	while (data.find("\r\n") != std::string::npos)
		data.replace(data.find("\r\n"), 2, "\n");
	if (data.size() > 1)
		messages = split(data, '\n');
}

std::queue<std::string> User::split(std::string &data, char separator)
{
	std::queue<std::string> new_messages;
	std::string::iterator i = data.begin();
	while (i != data.end())
	{
		while (i != data.end() && *i == separator) //walk through separator
			i++;
		if (i == data.end())
			break ;
		std::string::iterator j = std::find(i, data.end(), separator); //find separator in data
		if (j != data.end()) //push if separator found, including separator
			new_messages.push(std::string(i, j + 1));
		else //still push even if no separator (incomplete message) Is it needed?
			new_messages.push(std::string(i, j));
		i = j;
	}
	return new_messages;
}

std::string User::getNextMessage(void)
{
	std::string msg(messages.front().begin(), messages.front().end() - 1);
	messages.pop();

	return msg;
}


std::string User::getPassword( void ) const { return (_password); }
std::string User::getNick( void ) const { return (_nick); }
std::string User::getHostName( void ) const { return (_hostName); }
std::string User::getUserName( void ) const { return (_userName); }
std::string User::getRealName( void ) const { return (_realName); }
unsigned char User::getFlags( void ) const { return this->_flags; }
std::queue<std::string> User::getMessages( void ) const { return this->messages; }
int User::getSocketfd( void ) const { return this->_socketfd; }

void User::setPassword( std::string password ) { _password = password; }
void User::setNick( std::string nick ) { _nick = nick; }
void User::setHostName( std::string hostName ) { _hostName = hostName; }
void User::setUserName( std::string userName) { _userName = userName; }
void User::setRealName( std::string realName) { _realName = realName; }
void User::setFlags(unsigned char user_state_enum)
{
	// '|=' - set bit to needed value
	this->_flags |= user_state_enum;
}

std::vector<const Channel *> User::getChannels() const { return _channels; }

void User::clearFlags(unsigned char user_state_enum)
{
	this->_flags &= ~user_state_enum;
}

std::ostream	&operator<<( std::ostream &ostr, const User &instance )
{
	ostr	<< "\e[0;31m" << "User: " << instance.getNick() << std::endl
			<< "Socket:    " << instance.getSocketfd() << std::endl
			<< "User name: " << instance.getUserName() << std::endl
			<< "Host name: " << instance.getHostName() << std::endl
			<< "Real name: " << instance.getRealName() << "\e[0m" << std::endl;
	return (ostr);
}

bool User::operator==(const User &a)
{
	return (this->getNick() == a.getNick());
}

void User::addNewChannel(const Channel &channel) { _channels.push_back(&channel); }