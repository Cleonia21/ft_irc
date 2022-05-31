#pragma once

#include <string>
#include <vector>
#include <iomanip>

class Input
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::vector<std::string>	_params;

		std::string trimSpase(std::string str);
		void trimSubStr(std::string *out, std::string *in);
	public:
		Input(std::string userInput);
		~Input( void );

		Input(Input const &src);
		Input & operator=(Input const & src);

		std::string getPrefix(void) const;
		std::string getCommand(void) const;
		std::vector<std::string> getParams(void) const;
};

std::ostream	&operator<<( std::ostream &ostr, Input &instance );
