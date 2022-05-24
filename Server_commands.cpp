#include "Server.hpp"

int Server::pass(User &user, Input &input)
{
	// std::cout	<< "pass colled" << std::endl
	// 			<< input << std::endl;
	if (input.getParams()[0] == "") //если не указан пароль
		return (ERR_NEEDMOREPARAMS);
	if (user.getFlags() & USER_REGISTERED) //зареган ли этот пользователь раньше
		return (ERR_ALREADYREGISTRED);
	if (_password != input.getParams()[0]) //проверка на пароль сервера
		return (ERR_NEEDMOREPARAMS);//нужно сменить на правильный ответ
	//_users.push_back(new User); //добавляем нового пользователя
	//this->users.back()->setHostName(input->hostName); //ставим хост имя
	return (0);
}

int Server::nick(User &user, Input &input)
{
	// std::cout	<< "nick colled" << std::endl
	// 			<< input << std::endl;
	if (input.getParams()[0] == "") //если не указан ник
		return (ERR_NEEDMOREPARAMS);
	User *temp_user = searchUser(SRCH_HOSTNAME, user.getHostName());
	return (0);
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
}
