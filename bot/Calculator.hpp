#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Parser.hpp"

class Calculator
{
    std::string _result;


public:

    Calculator(std::string str);

    void    calculation(std::vector<std::string> &substrings, std::string signs);
    bool    checkToDouble(std::string str);
    bool	validityCheck(std::vector<std::string> substrings);
    bool    searchSumbolsInString(std::string str1, std::string str2);
    std::string getResult();
};