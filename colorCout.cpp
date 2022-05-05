#include "irc.hpp"

void redCout(string text)
{
    cout << "\e[0;31m" << text << "\e[0m" << std::endl;
}

void yellowCout(string text)
{
    cout << "\e[0;33m" << text << "\e[0m" << std::endl;
}

void greenCout(string text)
{
    cout << "\e[0;32m" << text << "\e[0m" << std::endl;
}