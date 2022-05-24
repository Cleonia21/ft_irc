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
	ERR_NONICKNAMEGIVEN = 431,		//":No nickname given"
	ERR_ERRONEUSNICKNAME = 432,		//"<nick> :Erroneus nickname"
	ERR_NICKNAMEINUSE = 433			//"<nick> :Nickname is already in use"
};

void redCout(std::string text);
void yellowCout(std::string text);
void greenCout(std::string text);

#endif
