#include "Parser.hpp"

Parser::Parser(std::string str, std::string delimiters)
{
    _substrings.push_back(str);
    for (size_t i = 0; i < delimiters.length(); i++)
    {
        std::vector<std::string> newSubstrings;

        for (size_t j = 0; j < _substrings.size(); j++)
        {
            std::vector<std::string> buf;
            buf = parseByCharacter(_substrings[j], delimiters[i]);
            newSubstrings.insert(newSubstrings.end(), buf.begin(), buf.end());
        }
        _substrings = newSubstrings;
    }
    erase(" ");
    erase("");
    // printSubstrings();
}

Parser::~Parser()
{
}

std::vector<std::string> Parser::getSubstrings()
{
    return _substrings;
}

std::vector<std::string> Parser::parseByCharacter(std::string str, char c)
{
    std::vector<std::string> substrings;

    size_t pos = 0;
    while ((pos = str.find(c)) != std::string::npos)
    {
        if (str[0] == c)
            pos = 1;
        substrings.push_back(str.substr(0, pos));
        str.erase(0, pos);
    }
    substrings.push_back(str);
    return (substrings);
}

void Parser::erase(std::string str)
{
    std::vector<std::string>::iterator j = _substrings.begin();
    while (j != _substrings.end())
    {
        if (*j == str)
            j = _substrings.erase(j);
        else
            j++;
    }
}

void Parser::printSubstrings()
{
    for (size_t i = 0; i < _substrings.size(); i++)
    {
        std::cout << "{" + _substrings[i] + "}" << std::endl;
    }
}

// int main()
// {
//     Parser("aa  2a    a 1b   11  211   cc   2c  1", " 12");

//     // std::string str = "123";
//     // std::cout << str.substr(0, 1) << std::endl;
//     // std::cout << str.erase(0, 1) << std::endl;
//     // std::cout << str << std::endl;
// }