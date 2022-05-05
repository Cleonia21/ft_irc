#include "irc.hpp"

User *searchUser(int key, string znch, vector<User*> users)
{
	string (User::*geter)( void );
	if (key == SRCH_NICK)
		geter = &User::getNick;
	else if (key == SRCH_HOSTNAME)
		geter = &User::getHostName;
	else if (key == SRCH_NAME)
		geter = &User::getName;
	else
	{
		redCout("Invalid key in searchUser");
		return (NULL);//NULL
	}
	for (int i = 0; i != users.size(); i++)
	{
		if ((users[i]->*geter)() == znch)
			return (users[i]);
	}
	return (NULL);//NULL
}

string decodingKey(int key, t_all *all, t_input *input)
{
	// errors
	if (key == ERR_NEEDMOREPARAMS)
		return (/* input->command + */" :Not enough parameters");
	if (key == ERR_ALREADYREGISTRED)
		return (":Unauthorized command (already registered)");

	// retvals
	if (key == RPL_EMPTY)
		return ("");
	// if (key == RPL_WELCOME)
	// 	return ("Welcome to the Internet Relay Network\n" + )
	// 	//"Welcome to the Internet Relay Network
    //            //<nick>!<user>@<host>"
	return ("Key not registerid(function decodingKey)");
}

int pass(t_all *all, t_input *input)
{
	if (input->param == "") //если не указан пароль
		return (ERR_NEEDMOREPARAMS);
	if (searchUser(SRCH_HOSTNAME, input->hostName, all->users) != NULL) //зареган ли этот пользователь раньше
		return (ERR_ALREADYREGISTRED);
	if (all->serverPassword != input->param) //проверка на пароль сервера
		return (ERR_NEEDMOREPARAMS);//нужно сменить на правильный ответ
	all->users.push_back(new User); //добавляем нового пользователя
	all->users.back()->setHostName(input->hostName); //ставим хост имя
	return (RPL_EMPTY);
}

int nick(t_all *all, t_input *input)
{
	// yellowCout("nick colled");
	if (input->param == "") //если не указан ник
		return (ERR_NEEDMOREPARAMS);
	User *user = searchUser(SRCH_HOSTNAME, input->hostName, all->users);

	return (0);
}

t_all *initial( void )
{
	t_all *all = new t_all;

	all->serverPassword = "1111";

	all->commands = (int(**)(t_all*, t_input*))malloc(sizeof(int(*)(t_all*, t_input*)) * 2);
	all->commands[0] = pass;
	all->commands[1] = nick;

	return (all);
}

int main()
{
	t_all *all = initial();
	
	execution(all, "PASS :123 erfnjer");

	return (0);
}
