#include "irc.hpp"

t_input *parser(string input)
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
