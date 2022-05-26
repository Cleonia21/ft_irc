#include "irc.hpp"

// user - кому ответ/ошибка отправляется
// code - номер ответа/ошибки (доступные находятся в irc.hpp)
// arg* - необходимые аргументы для ответа/ошибки
int sendServerReply (const User &user, int code,
		const std::string &arg1, const std::string &arg2,
		const std::string &arg3, const std::string &arg4)
{
	std::stringstream stream;
	std::string reply;
	if (code < 10)
		stream << 0 << 0;
	stream << code;
	// 'reply' example: ":IRCSERVER 372 Bob "
	reply = ":" + std::string(ircName) + " " + stream.str() + " " + user.getNick() + " ";
	switch (code)
	{
		/*
		 * Error cases
		 */

		case ERR_NOSUCHNICK:
			reply += arg1 + " :No such nick/channel\n";
			break;
		case ERR_NOSUCHSERVER:
			reply += arg1 + " :No such server\n";
			break;
		case ERR_NOSUCHCHANNEL:
			reply += arg1 + " :No such channel\n";
			break;
		case ERR_CANNOTSENDTOCHAN:
			reply += arg1 + " :Cannot send to channel\n";
			break;
		case ERR_TOOMANYCHANNELS:
			reply += arg1 + " :You have joined too many channels\n";
			break;
		case ERR_TOOMANYTARGETS:
			reply += arg1 + " :Duplicate recipients. No message delivered\n";
			break;
		case ERR_NORECIPIENT:
			reply += ":No recipient given (" + arg1 + ")\n";
			break;
		case ERR_NOTEXTTOSEND:
			reply += ":No text to send\n";
			break;
		case ERR_NOTOPLEVEL: //Don't know what that's supposed to be
			reply += arg1 + " :No toplevel domain specified\n";
			break;
		case ERR_WILDTOPLEVEL: //Don't know what that's supposed to be
			reply += arg1 + " :Wildcard in toplevel domain\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			reply += arg1 + " :Unknown command\n";
			break;
		case ERR_NONICKNAMEGIVEN:
			reply += ":No nickname given\n";
			break;
		case ERR_ERRONEUSNICKNAME:
			reply += arg1 + " :Erroneus nickname\n";
			break;
		case ERR_NICKNAMEINUSE:
			reply += arg1 + " :Nickname is already in use\n";
			break;
		case ERR_USERNOTINCHANNEL:
			reply += arg1 + " " + arg2 + " :They aren't on that channel\n";
			break;
		case ERR_NOTONCHANNEL:
			reply += arg1 + " :You're not on that channel\n";
			break;
		case ERR_USERONCHANNEL:
			reply += arg1 + " " + arg2 + " :is already on channel\n";
			break;
		case ERR_NOTREGISTERED:
			reply += " :You have not registered\n";
			break;
		case ERR_NEEDMOREPARAMS:
			reply += arg1 + " :Not enough parameters\n";
			break;
		case ERR_ALREADYREGISTRED:
			reply += ":You may not reregister\n";
			break;
		case ERR_PASSWDMISMATCH:
			reply += ":Password incorrect\n";
			break;
		case ERR_CHANNELISFULL:
			reply += arg1 + " :Cannot join channel (+l)\n";
			break;
		case ERR_INVITEONLYCHAN:
			reply += arg1 + " :Cannot join channel (+i)\n";
			break;
		case ERR_BANNEDFROMCHAN:
			reply += arg1 + " :Cannot join channel (+b)\n";
			break;
		case ERR_BADCHANNELKEY:
			reply += arg1 + " :Cannot join channel (+k)\n";
			break;
		case ERR_NOPRIVILEGES:
			reply += ":Permission Denied- You're not an IRC operator\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			reply += arg1 + " :You're not channel operator\n";
			break;
		case ERR_CANTKILLSERVER:
			reply += ":You cant kill a server!\n";
			break;
		case ERR_NOOPERHOST:
			reply += ":No O-lines for your host\n";
			break;

		/*
		 * Reply cases
		 */


			
		case RPL_WELCOME:
			//"Welcome to the Internet Relay Network <nick>!<user>@<host>"
			reply += "Welcome to the Internet Relay Network " + arg1 + "!" + arg2 + "@" + arg3 + "\n";
			break;
		case RPL_YOURHOST:
			reply += "Your host is " + arg1 + ", running version " + arg2 + "\n";
			break;
		case RPL_CREATED:
			reply += "This server was created " + arg1 + "\n";
			break;
		case RPL_MYINFO:
			reply += arg1 + " " + arg2 + " " + arg3 + " " + arg4 + "\n";
			break;
		case RPL_ISON:
			reply += ":" + arg1 + "\n";
			break;
		case RPL_NOTOPIC:
			reply += arg1 + " :No topic is set\n";
			break;
		case RPL_TOPIC:
			reply += arg1 + " :" + arg2 + "\n";
			break;
		case RPL_INVITING:
			reply += arg1 + " " + arg2 + "\n";
			break;
		case RPL_MOTDSTART:
			reply += ":- " + arg1 + " Message of the day - \n";
			break;
		case RPL_MOTD:
			reply += ":- " + arg1 + "\n";
			break;
		case RPL_ENDOFMOTD:
			reply += ":End of /MOTD command\n";
			break;
		case RPL_YOUREOPER:
			reply += ":You are now an IRC operator\n";
			break;
		case RPL_LUSERCLIENT:
			reply += ":There are " + arg1 + " users and " + arg2;
			reply += " invisible on " + arg3 + " servers\n";
			break;
		case RPL_LUSEROP:
			reply += arg1 + " :operator(s) online\n";
			break;
		case RPL_LUSERUNKNOWN:
			reply += arg1 + " :unknown connection(s)\n";
			break;
		case RPL_LUSERCHANNELS:
			reply += arg1 + " :channels formed\n";
			break;
		case RPL_LUSERME:
			reply += ":I have " + arg1 + " clients and " + arg2 + " servers\n";
			break;
        case RPL_NAMREPLY:
            reply += arg1 + " :" + arg2 + "\n";
            break;
        case RPL_ENDOFNAMES:
            reply += arg1 + " :End of /NAMES list\n";
            break;
		default:
			reply += "UNKNOWN REPLY\n";
			break;
	}
	send(user.getSocketfd(), reply.c_str(), reply.size(), 0);

	if (code > 400)
		return (-1);
	return (0);
}
