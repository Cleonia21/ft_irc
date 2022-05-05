#include "irc.hpp"

void execution(t_all *all, string input)
{
	t_input *input_info = parser(input);

	all->commands[input_info->command] (all, input_info);

}
