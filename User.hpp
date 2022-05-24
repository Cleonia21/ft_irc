#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
#include <sys/socket.h>
#include "Input.hpp"
#include "Channel.hpp"

enum User_states
{
	USER_REGISTERED =	0b00000001,
	USER_DISCONNECTED =	0b00000010
};

class Channel;

class User
{
	private:
		std::string		_nick;
		std::string		_hostName; //it is an ip address, but maybe we should mask it
		std::string		_userName;
		std::string		_realName;
		std::string		_password;
		int				_socketfd;
		unsigned char	_flags;
		std::queue<std::string>		messages;
        std::vector<const Channel *>  _channels;

		//unused
		User( void );
		User(User const &src);
		User & operator=(User const & src);
	public:
		User(std::string hostName, int new_fd);
		~User( void );

		void processMessage(void);
		std::string getNextMessage( void );

		void setPassword( std::string );
		void setNick( std::string );
		void setHostName( std::string );
		void setUserName( std::string );
		void setRealName( std::string );
		void setFlags(unsigned char user_state_enum);
		void clearFlags(unsigned char user_state_enum);
        void addNewChannel(const Channel& channel);

		std::string getPassword( void ) const;
		std::string getNick( void ) const;
		std::string getHostName( void ) const;
		std::string getUserName( void ) const;
		std::string getRealName( void ) const;
		unsigned char getFlags( void ) const;
        std::vector<const Channel *> getChannels() const;
		std::queue<std::string> getMessages( void ) const;
		int getSocketfd( void ) const;

        //functions
        //сделал статик, чтобы можно было пользоваться откуда угодно, функция будет часто нужна
        static std::queue<std::string>		split(std::string &data, char separator);
};

std::ostream	&operator<<( std::ostream &ostr, User &instance );
