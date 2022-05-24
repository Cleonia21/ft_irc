#ifndef IRC_H
#define IRC_H

#include <string>
#include <iostream>
#include <sstream>

enum e_search
{
	SRCH_NICK,
	SRCH_HOSTNAME,
	SRCH_REALNAME
};

enum e_errors
{
	ERR_NEEDMOREPARAMS = 461,		//"<command> :Not enough parameters"
	ERR_ALREADYREGISTRED = 462,		//":Unauthorized command (already registered)"
    ERR_NOSUCHCHANNEL = 403,        // "<channel name> :No such channel"
    ERR_TOOMANYCHANNELS = 405,      //"<channel name> :You have joined too many channels"
    ERR_BADCHANNELKEY = 475,         //"<channel> :Cannot join channel (+k)"
    ERR_INVITEONLYCHAN = 473,
    ERR_BANNEDFROMCHAN = 474
};

void redCout(std::string text);
void yellowCout(std::string text);
void greenCout(std::string text);

#endif
