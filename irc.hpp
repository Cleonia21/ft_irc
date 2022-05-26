#ifndef IRC_H
#define IRC_H

#include <string>
#include <iostream>
#include <sstream>
#include "User.hpp"

const char ircName[] = "IRCSERVER";

enum e_search
{
	SRCH_NICK,
	SRCH_HOSTNAME,
	SRCH_REALNAME
};

enum e_errors
{
	ERR_ = 1,
	RPL_WELCOME =				001, //"Welcome to the Internet Relay Network <nick>!<user>@<host>"
    RPL_YOURHOST =				002, //"Your host is <servername>, running version <ver>"
    RPL_CREATED =				003, //"This server was created <date>"
    RPL_MYINFO =				004, //"<servername> <version> <available user modes> <available channel modes>"
	ERR_NOSUCHNICK =			401, // "<nickname> :No such nick/channel"
	ERR_NOSUCHSERVER =			402,
	ERR_NOSUCHCHANNEL =			403, // "<channel name> :No such channel"
	ERR_CANNOTSENDTOCHAN =		404,
	ERR_TOOMANYCHANNELS =		405,
	ERR_WASNOSUCHNICK =			406,
	ERR_TOOMANYTARGETS =		407,
	ERR_NOORIGIN =				409,
	ERR_NORECIPIENT =			411,
	ERR_NOTEXTTOSEND =			412,
	ERR_NOTOPLEVEL =			413,
	ERR_WILDTOPLEVEL =			414,
	ERR_UNKNOWNCOMMAND =		421,
	ERR_NOMOTD =				422,
	ERR_NOADMININFO =			423,
	ERR_FILEERROR =				424,
	ERR_NONICKNAMEGIVEN = 		431, //":No nickname given"
	ERR_ERRONEUSNICKNAME = 		432, //"<nick> :Erroneus nickname"
	ERR_NICKNAMEINUSE = 		433, //"<nick> :Nickname is already in use"
	ERR_NICKCOLLISION =			436,
	ERR_USERNOTINCHANNEL =		441,
	ERR_NOTONCHANNEL =			442,
	ERR_USERONCHANNEL =			443,
	ERR_NOLOGIN =				444,
	ERR_SUMMONDISABLED =		445,
	ERR_USERSDISABLED =			446,
	ERR_NOTREGISTERED =			451,
	ERR_NEEDMOREPARAMS =		461, //"<command> :Not enough parameters"
	ERR_ALREADYREGISTRED =		462, //":Unauthorized command (already registered)"
	ERR_NOPERMFORHOST =			463,
	ERR_PASSWDMISMATCH =		464,
	ERR_YOUREBANNEDCREEP =		465,
	ERR_KEYSET =				467,
	ERR_CHANNELISFULL =			471,
	ERR_UNKNOWNMODE =			472, // "<char> :is unknown mode char to me"
	ERR_INVITEONLYCHAN =		473,
	ERR_BANNEDFROMCHAN =		474,
	ERR_BADCHANNELKEY =			475,
	ERR_NOPRIVILEGES =			481,
	ERR_CHANOPRIVSNEEDED =		482, // "<channel> :You're not channel operator"
	ERR_CANTKILLSERVER =		483,
	ERR_NOOPERHOST =			491,
	ERR_UMODEUNKNOWNFLAG =		501,
	ERR_USERSDONTMATCH =		502
};

enum e_replies
{
	RPL_NONE =				300,
	RPL_USERHOST =			302,
	RPL_ISON =				303,
	RPL_AWAY =				301,
	RPL_UNAWAY =			305,
	RPL_NOWAWAY =			306,
	RPL_WHOISUSER =			311,
	RPL_WHOISSERVER =		312,
	RPL_WHOISOPERATOR =		313,
	RPL_WHOISIDLE =			317,
	RPL_ENDOFWHOIS =		318,
	RPL_WHOISCHANNELS =		319,
	RPL_WHOWASUSER =		314,
	RPL_ENDOFWHOWAS =		369,
	RPL_LISTSTART =			321,
	RPL_LIST =				322,
	RPL_LISTEND =			323,
	RPL_CHANNELMODEIS =		324,
	RPL_NOTOPIC =			331,
	RPL_TOPIC =				332,
	RPL_INVITING =			341,
	RPL_SUMMONING =			342,
	RPL_VERSION =			351,
	RPL_WHOREPLY =			352,
	RPL_ENDOFWHO =			315,
	RPL_NAMREPLY =			353,
	RPL_ENDOFNAMES =		366,
	RPL_LINKS =				364,
	RPL_ENDOFLINKS =		365,
	RPL_BANLIST =			367,
	RPL_ENDOFBANLIST =		368,
	RPL_INFO =				371,
	RPL_ENDOFINFO =			374,
	RPL_MOTDSTART =			375,
	RPL_MOTD =				372,
	RPL_ENDOFMOTD =			376,
	RPL_YOUREOPER =			381,
	RPL_REHASHING =			382,
	RPL_TIME =				391,
	RPL_USERSSTART =		392,
	RPL_USERS =				393,
	RPL_ENDOFUSERS =		394,
	RPL_NOUSERS =			395,
	RPL_TRACELINK =			200,
	RPL_TRACECONNECTING =	201,
	RPL_TRACEHANDSHAKE =	202,
	RPL_TRACEUNKNOWN =		203,
	RPL_TRACEOPERATOR =		204,
	RPL_TRACEUSER =			205,
	RPL_TRACESERVER =		206,
	RPL_TRACENEWTYPE =		208,
	RPL_TRACELOG =			261,
	RPL_STATSLINKINFO =		211,
	RPL_STATSCOMMANDS =		212,
	RPL_STATSCLINE =		213,
	RPL_STATSNLINE =		214,
	RPL_STATSILINE =		215,
	RPL_STATSKLINE =		216,
	RPL_STATSYLINE =		218,
	RPL_ENDOFSTATS =		219,
	RPL_UMODEIS =			221,
	RPL_STATSLLINE =		241,
	RPL_STATSUPTIME =		242,
	RPL_STATSOLINE =		243,
	RPL_STATSHLINE =		244,
	RPL_LUSERCLIENT =		251,
	RPL_LUSEROP =			252,
	RPL_LUSERUNKNOWN =		253,
	RPL_LUSERCHANNELS =		254,
	RPL_LUSERME =			255,
	RPL_ADMINME =			256,
	RPL_ADMINLOC1 =			257,
	RPL_ADMINLOC2 =			258,
	RPL_ADMINEMAIL =		259
};

class User;

int sendServerReply (const User &user, int code,
		const std::string &arg1 = "", const std::string &arg2 = "",
		const std::string &arg3 = "", const std::string &arg4 = "");

void redCout(std::string text);
void yellowCout(std::string text);
void greenCout(std::string text);

#endif
