#include "Server.hpp"

int Server::pass(User &user, Input &input)
{
	if (input.getParams().size() == 0) //если не указан пароль
		sendServerReply(user, ERR_NEEDMOREPARAMS, input.getCommand());
	else if (user.getFlags() & USER_REGISTERED) //зареган ли этот пользователь раньше
		sendServerReply(user, ERR_ALREADYREGISTRED);
	else
		user.setPassword(input.getParams()[0]);
	return (0);
}

int Server::nick(User &user, Input &input)
{
	if (input.getParams().size() == 0) //если не указан ник
		sendServerReply(user, ERR_NEEDMOREPARAMS, input.getCommand());
	else if (!isNickValid(input.getParams()[0]) || input.getParams()[0] == ircName) //Чек на символы
		sendServerReply(user, ERR_ERRONEUSNICKNAME, input.getParams()[0]);
	else if (containsNickname(input.getParams()[0])) //Чек на присутствие другого юзера с ником
		sendServerReply(user, ERR_NICKNAMEINUSE, input.getParams()[0]);
	else
	{
		if (user.getFlags() & USER_REGISTERED)
		{
			/*
			 * Need to notify other Users in channels of this user nickname change
			 */
		}
		user.setNick(input.getParams()[0]);
	}
	return (checkForRegistration(user));
}

int Server::join(User &user, Input &input) {

    if (input.getParams()[0] == "")
        return ERR_NEEDMOREPARAMS;
    else
    {
        std::queue<std::string>	channels = User::split(input.getParams()[0], ',');
        std::queue<std::string>	keys;
        if (input.getParams().size() > 1)
            keys = split(input.getParams()[1], ',');
        for (; channels.size() > 0; channels.pop())
        {
            std::string	key = keys.size() ? keys.front() : "";
            if (keys.size() > 0)
                keys.pop();
            if (!Channel::isChannelNameCorrect(channels.front()))
                return ERR_NOSUCHCHANNEL;
            else if (user.getChannels().size() >= 10)
                return ERR_TOOMANYCHANNELS;
            else {
                try
                {
                    Channel	*tmp = _channels.at(channels.front());
                    tmp->connect(user, key);
                    return (1);
                }
                catch(const std::exception& e)
                {
                    channels[name] = new Channel(name, user, key);
                }
                return (1);
            }
            }


                if (connectToChannel(user, chans.front(), key) == 1)
                user.addChannel(*(channels.at(chans.front())));
        }
    }
    return 0;
int Server::user(User &user, Input &input)
{
	if (input.getParams().size() < 4)
		sendServerReply(user, ERR_NEEDMOREPARAMS, input.getCommand());
	else if (user.getFlags() & USER_REGISTERED)
		sendServerReply(user, ERR_ALREADYREGISTRED);
	else
	{
		user.setUserName(input.getParams()[0]);
		user.setRealName(input.getParams()[3]);
	}
	return (checkForRegistration(user));
}

// Суть данного чека: Проверка пароля только если и ник и имя было указано.
// Если же пароль не верен (была или не была использована команда PASS - неважно), то клиент отключается
int Server::checkForRegistration(User &user)
{
	if (user.getNick().size() == 0 || user.getUserName().size() == 0) //Чек на наличие ника и имени
		return (0);
	if (!(_password.size() == 0 || user.getPassword() == _password)) //Чек на пароль
		return (SERVER_DISCONNECT);

	if (!(user.getFlags() & USER_REGISTERED))
	{
		user.setFlags(USER_REGISTERED);
		/////// Send MOTD on registration
	}
	return (0);
}
