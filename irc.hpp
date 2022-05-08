#ifndef IRC_H
#define IRC_H

#include <string>
#include <iostream>

enum e_search
{
	SRCH_NAME,
	SRCH_NICK,
	SRCH_HOSTNAME,
	SRCH_REALNAME
};

enum e_errors
{
	ERR_NEEDMOREPARAMS = 461,		//"<command> :Not enough parameters"
	ERR_ALREADYREGISTRED = 462		//":Unauthorized command (already registered)"
};

void redCout(std::string text);
void yellowCout(std::string text);
void greenCout(std::string text);

// template<typename T>
// typename T::iterator easyfind(T &elem, int znch)
// {
//     typename T::iterator result;

//     result = find(elem.begin(), elem.end(), znch);
//     if (result == elem.end())
//         throw std::logic_error("easyfind: znch not fond");
//     return (result);
// }


#endif
