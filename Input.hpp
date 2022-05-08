#pragma once

#include <string>
#include <vector>

class Input
{
	private:
		std::string					prefix;
		std::string					command;
		std::vector<std::string>	params;

		Input(void);
		Input(Input const &src);
		Input & operator=(Input const & src);
	public:
		Input(std::string User_input); //Here goes parser
		~Input(void);

		std::string getPrefix(void) const;
		std::string getCommand(void) const;
		std::vector<std::string> getParams(void) const;
};
