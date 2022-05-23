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


