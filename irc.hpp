#ifndef IRC_H
#define IRC_H

#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

class Channel;
class User;

#include "User.hpp"
#include "Channel.hpp"

enum e_commands
{
	PASS,
	NICK,
	USER,
	PRIVMSG,
	NOTICE,
	JOIN,
	KICK
};

enum e_search
{
	SRCH_NAME,
	SRCH_NICK,
	SRCH_HOSTNAME,
	SRCH_REALNAME
};

enum e_errors
{
	ERR_NEEDMOREPARAMS = 461,		//"<command> :Not enough parameters"
	ERR_ALREADYREGISTRED = 462		//":Unauthorized command (already registered)"
};

enum retvals_commands
{
	RPL_EMPTY = 0,
	RPL_WELCOME = 001				//"Welcome to the Internet Relay Network\n<nick>!<user>@<host>"
};

typedef struct s_input
{
	int command;
	string prefix;
	string param;
	string hostName;
}   t_input;

typedef struct s_all
{
	vector<User*>	users;
	vector<Channel*>	channels;
	string			serverPassword;
	int (**commands) (struct s_all *, t_input *);
}   t_all;

t_input *parser(string input);
void execution(t_all *all, string input);

void redCout(string text);
void yellowCout(string text);
void greenCout(string text);

// template<typename T>
// typename T::iterator easyfind(T &elem, int znch)
// {
//     typename T::iterator result;

//     result = find(elem.begin(), elem.end(), znch);
//     if (result == elem.end())
//         throw std::logic_error("easyfind: znch not fond");
//     return (result);
// }


#endif
