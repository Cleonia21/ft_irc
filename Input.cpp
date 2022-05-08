#include "Input.hpp"

Input::Input(std::string User_input)
{
	//Parser here
}

Input::~Input(void)
{

}

std::string Input::getPrefix(void) const
{
	return (this->prefix);
}

std::string Input::getCommand(void) const
{
	return (this->command);
}

std::vector<std::string> Input::getParams(void) const
{
	return (this->params);
}


/*
t_input *parser(std::string input)
{
	t_input *input_info;

	input_info = new t_input;

	int i = input.find("PASS");
	if (i != -1)
	{
		input_info->command = PASS;
		input.erase(i, 4);
	}
	i = input.find(":");
	if (i != -1)
	{
		int j = input.find(" ", i);
		input_info->prefix = input.substr(i, j);
		input.erase(i, j);
	}
	input_info->param = input;
	return (input_info);
}
*/
