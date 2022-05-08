#include "irc.hpp"

void redCout(std::string text)
{
	std::cout << "\e[0;31m" << text << "\e[0m" << std::endl;
}

void yellowCout(std::string text)
{
	std::cout << "\e[0;33m" << text << "\e[0m" << std::endl;
}

void greenCout(std::string text)
{
	std::cout << "\e[0;32m" << text << "\e[0m" << std::endl;
}
