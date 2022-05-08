#include "Server.hpp"

int Server::pass(User &user, Input &input)
{
	if (input.getParams()[0] == "") //если не указан пароль
		return (ERR_NEEDMOREPARAMS);
	if (searchUser(SRCH_HOSTNAME, user.getHostName(), this->users) != NULL) //зареган ли этот пользователь раньше
		return (ERR_ALREADYREGISTRED);
	if (this->password != input.getParams()[0]) //проверка на пароль сервера
		return (ERR_NEEDMOREPARAMS);//нужно сменить на правильный ответ
	this->users.push_back(new User); //добавляем нового пользователя
//	this->users.back()->setHostName(input->hostName); //ставим хост имя
	return (0);
}

int Server::nick(User &user, Input &input)
{
	// yellowCout("nick colled");
	if (input.getParams()[0] == "") //если не указан ник
		return (ERR_NEEDMOREPARAMS);
	User *temp_user = searchUser(SRCH_HOSTNAME, user.getHostName(), this->users);

	return (0);
}


