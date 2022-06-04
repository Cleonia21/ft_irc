#include "Input.hpp"

Input::Input(std::string userInput)
{
	if (userInput.length() > 510)
		userInput.resize(510);
	userInput = trimSpase(userInput);
	if (userInput[0] == ':')
	{
		userInput.erase(0, 1);
		trimSubStr(&userInput, &_prefix);
	}
	trimSubStr(&userInput, &_command);
	for (std::string i; userInput.length();)
	{
		if (userInput[0] == ':')
		{
			userInput.erase(0, 1);
			_params.push_back(userInput);
			break ;
		}
		else
			trimSubStr(&userInput, &i);
		_params.push_back(i);
	}
}

void Input::trimSubStr(std::string *out, std::string *in)
{
	size_t i = 0;
	for (; i < (*out).length(); i++)
		if ((*out)[i] == ' ')
			break ;
	*in = out->substr(0, i);
	for (; i < (*out).length(); i++)
		if ((*out)[i] != ' ')
			break ;
	out->erase(0, i);
}

std::string Input::trimSpase(std::string str)
{
	size_t i, j;
	for (i = 0; i < str.length(); i++)
		if (str[i] != ' ')
			break ;
	for (j = str.length() - 1; j > 0; j--)
		if (str[j] != ' ')
			break;
	return (str.substr(i, j - i + 1));
}

std::string Input::getPrefix(void) const
{
	return (this->_prefix);
}

std::string Input::getCommand(void) const
{
	return (this->_command);
}

std::vector<std::string> Input::getParams(void) const
{
	return (this->_params);
}

Input::~Input( void ) {}

std::ostream	&operator<<( std::ostream &ostr, Input &instance )
{
	ostr	<< "prefix {" << instance.getPrefix() << "} command {"
			<< instance.getCommand() << "}" << std::endl
			<< "param list: ";
	std::vector<std::string> params = instance.getParams();
	for (size_t i = 0; i < params.size(); i++)
		ostr << "\"" << params[i] << "\" ";
	return (ostr);
}
