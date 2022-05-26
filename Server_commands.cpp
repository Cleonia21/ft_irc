#include "Server.hpp"
#include "Channel.hpp"

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
		sendWelcome(user);
		sendMOTD(user);
	}
	return (0);
}

void Server::sendWelcome(const User &user) const
{
	std::string version = "0.678";
	std::string date = "Today";
	sendServerReply(user, RPL_WELCOME, user.getNick(), user.getUserName(), user.getHostName());
	sendServerReply(user, RPL_YOURHOST, ircName, version);
	sendServerReply(user, RPL_CREATED, date);
	sendServerReply(user, RPL_MYINFO, ircName, version, "i", "biklmnopstv");
}

void Server::sendMOTD(const User &user) const
{
	if (_motd.size() == 0)
		sendServerReply(user, ERR_NOMOTD);
	else
	{
		sendServerReply(user, RPL_MOTDSTART, ircName);
		for (size_t i = 0; i < _motd.size(); ++i)
			sendServerReply(user, RPL_MOTD, _motd[i]);
		sendServerReply(user, RPL_ENDOFMOTD);
	}
}

/*
 ****************************************************************************
 */

int Server::join(User &user, Input &input) {

	if (input.getParams()[0] == "")
        sendServerReply(user, ERR_NEEDMOREPARAMS, "JOIN");

	std::queue<std::string> channels = User::split(input.getParams()[0], ',');
	std::queue<std::string> keys;
	if (input.getParams().size() > 1)
		keys = User::split(input.getParams()[1], ',');

	for (; !channels.empty(); channels.pop()) {

		std::string channelName = channels.front();
		std::string channelKey;
		if (!keys.empty()) {
			channelKey = keys.front();
			keys.pop();
		} else
			channelKey = "";

		if (!Channel::isChannelNameCorrect(channelName))
            sendServerReply(user, ERR_NOSUCHCHANNEL, channelName);
		else if (user.getChannels().size() >= 10)
            sendServerReply(user, ERR_TOOMANYCHANNELS, channelName);
        else {
            try {
                Channel *tmp = _channels.at(channelName);
                tmp->connect(user, channelKey);
            }
            catch (const std::exception &e) {
                _channels[channelName] = new Channel(channelName, user, channelKey);
            }
            user.addNewChannel(*(_channels.at(channelName)));
        }
	}
	return 0;
}

int		Server::kick(User &user, Input &input)
{
	std::string channelName = input.getParams()[0];
	std::string nickName = input.getParams()[1];

	if (input.getParams().size() < 2)
        sendServerReply(user, ERR_NEEDMOREPARAMS, "KICK");
	else if (_channels.find(channelName) == _channels.end())
        sendServerReply(user, ERR_NOSUCHCHANNEL, channelName);
    else if (!_channels.at(channelName)->isOperator(user))
        sendServerReply(user, ERR_CHANOPRIVSNEEDED, channelName);
    else if (!_channels.at(channelName)->isChannelUser(user.getNick()))
        sendServerReply(user, ERR_NOTONCHANNEL, channelName);
    else if (!containsNickname(nickName))
        sendServerReply(user, ERR_NOSUCHNICK, nickName);
    else if (!_channels.at(channelName)->isChannelUser(nickName))
        sendServerReply(user, ERR_USERNOTINCHANNEL, nickName, channelName);
    else {
        Channel *channel = _channels.at(channelName);
        std::string message = " KICK " + channel->getName() + " " + nickName + " ";

        if (input.getParams().size() > 2)
            message += input.getParams()[2];
        else
            message += ":no reason";

        std::cout << message << "\n";
        channel->sendNotification(message, user);
        User *userToBeKicked;
        for (int i = 0; i < _users.size(); i++)
            if (_users[i]->getNick() == nickName)
                userToBeKicked = _users[i];
        channel->disconnect(*(userToBeKicked));
    }
    return 0;
}



bool checkFlagsForValid(std::string _flags)
{
	if ((_flags[0] != '+' && _flags[0] != '-')
		|| _flags.size() > 7)
		return false;
	const char *flags = _flags.c_str() + 1; // пропускаем +-
	const char *dict;
	if (_flags.size() == 1)
		dict = "opsitnmlbvk";
	else
		dict = "psitnm";
	// проверка на отличные символы в flags от символов в dict
	if (strspn(flags, dict) != _flags.size() - 1)
		return false;
	// проверка на повторы
	for (; *dict; dict++)
		if (_flags.find(*dict) != _flags.rfind(*dict))
			return false;
	return true;
}

int Server::mode(User &user, Input &input)
{
	if (input.getParams().size() < 2)
		return sendServerReply(user, ERR_NEEDMOREPARAMS, input.getCommand());

	std::string object = input.getParams()[0];
	std::string flags = input.getParams()[1];
	if (object[0] == '#' || object[0] == '&') // chanel
	{
		object.erase(0, 1);
		Channel *channel;
		try { channel = _channels.at(object); }
		catch (const std::exception &e) { return sendServerReply(user, ERR_NOSUCHCHANNEL, object); }
		if (!channel->isOperator(user))
			return sendServerReply(user, ERR_CHANOPRIVSNEEDED, channel->getName());
		if (!checkFlagsForValid(flags))
			return sendServerReply(user, ERR_UNKNOWNMODE, flags);

		for (int i = 1; flags[i] != '\0'; i++)
		{
			unsigned char flag;
			std::string argument;
			switch (flags[i])
			{
			case 'p':
				flag = CHL_PRIVATE;
				break;
			case 's':
				flag = CHL_SECRET;
				break;
			case 'i':
				flag = CHL_INVITEONLY;
				break;
			case 't':
				flag = CHL_TOPICSET;
				break;
			case 'n':
				flag = CHL_NOMSGOUT;
				break;
			case 'm':
				flag = CHL_MODERATED;
				break;
			if (input.getParams().size() != 3)
				return sendServerReply(user, ERR_NEEDMOREPARAMS, input.getCommand());
			argument = input.getParams()[2];
			User *tmpUser;
			case 'o':
				tmpUser = this->searchUser(SRCH_NICK, argument);
				if (!tmpUser)
					return sendServerReply(user, ERR_NOSUCHNICK, argument);
				if (flags[0] == '-')
					channel->deleteOperator(*tmpUser);
				else
					channel->addOperator(*tmpUser);
				continue;
			case 'l':
				channel->setLimit(std::stoi(argument));
				continue;
			case 'b':
				if (flags[0] == '-')
					channel->addBanMask(argument);
				else
					channel->removeBanMask(argument);
			case 'v':
				// realization
				continue;
			case 'k':
				if (flags[0] == '-')
					channel->setPass(argument);
				else
					channel->removePass();
			}
			if (flags[0] == '-')
				channel->clearFlag(flag);
			else
				channel->setFlag(flag);
		}
	}
	return (0);
}

/*
o – брать/давать привилегии операторов канала
p – флаг приватности канала;
s – флаг секретности канала;
i – флаг канала invite-only;
t – при установке этого флага, менять топик могут только операторы;
n – запрещает сообщения на канал от посторонних клиентов;
m – модерируемый канал;
l – установка ограничения на количество пользователей;
b – установка маски бана;
v – брать/давать возможность голоса при модерируемом режиме;
k – установка пароля на канал.
*/

static int pm_or_notice(User &user, Input &input, int code, int silent)
{
	if (silent)
		return (-1);
	return (sendServerReply(user, code, input.getCommand()));
}

static int pm_or_notice(User &user, Input &input, int code, int silent, const std::string &name)
{
	if (silent)
		return (-1);
	return (sendServerReply(user, code, name));
}


int Server::sendPM(User &user, Input &input, int silent)
{
	if (input.getParams().size() == 0) //Only command present
		return (pm_or_notice(user, input, ERR_NORECIPIENT, silent));
	if (input.getParams().size() == 1) //Only command and recipient present
		return (pm_or_notice(user, input, ERR_NEEDMOREPARAMS, silent));
	if (input.getParams()[1].size() == 0) //Empty message (just "")
		return (pm_or_notice(user, input, ERR_NOTEXTTOSEND, silent));

	std::queue<std::string> receivers = User::split(input.getParams()[0], ',', 1);
	std::set<std::string> finalReceivers;
	std::pair<std::set<std::string>::iterator, bool> check_pair;

	while (receivers.size() != 0)
	{
		int check; //переменная, показывающая что сервер отправил ошибку
		check_pair = finalReceivers.insert(receivers.front());
		if (check_pair.second == false)
		{
			receivers.pop();
			continue ;
		}
		if (receivers.front()[0] == '#' || receivers.front()[0] == '&') //Для каналов
		{
			check = 0;
			std::string channel = receivers.front();
			if (!containsChannel(channel)) //Нет канала
				check = pm_or_notice(user, input, ERR_NOSUCHNICK, silent, channel);
			else if (!_channels[channel]->isChannelUser(user.getNick())) //Нет юзера на канале
			{
				if (_channels[channel]->getFlags() & CHL_NOMSGOUT) //Не разрешены внешние сообщения
					check = pm_or_notice(user, input, ERR_CANNOTSENDTOCHAN, silent);
				if (_channels[channel]->getFlags() & CHL_MODERATED) //Нельзя писать на модерируемый
					check = pm_or_notice(user, input, ERR_CANNOTSENDTOCHAN, silent);
			}
			//Юзер на канале, остается узнать есть ли права писать, если канал модерируется
			else if (_channels[channel]->getFlags() & CHL_MODERATED
					&& !_channels[channel]->isOperator(user)) //добавить чек на спикера
				check = pm_or_notice(user, input, ERR_CANNOTSENDTOCHAN, silent);

			//Если в верхние if не зашли (т.е. все правильно), то отправляем сообщение
			if (!check)
			{
				std::string msg = input.getCommand() + " " + channel + " :" + input.getParams()[1] + "\n";
				_channels[channel]->sendNotification(msg, user);
			}
		}
		else //Для юзеров
		{
			check = 0;
			if (!containsNickname(receivers.front()))
				check = pm_or_notice(user, input, ERR_NOSUCHNICK, silent, receivers.front());
			if (!check)
			{
				User *recipient = searchUser(SRCH_NICK, receivers.front());
				std::string msg = ":" + user.getMask() + " " + input.getCommand() + " " + recipient->getNick() + " :" + input.getParams()[1] + "\n";
				recipient->sendMessage(msg);
			}
		}
		receivers.pop();
	}

	return (0);
}

int Server::privmsg(User &user, Input &input)
{
	return sendPM(user, input, 0);
}

int Server::notice(User &user, Input &input)
{
	return sendPM(user, input, 1);
}
