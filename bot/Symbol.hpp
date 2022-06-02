#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

class Symbol
{
    std::vector<std::string> _c;

public:
    Symbol(std::vector<std::string> c);
    ~Symbol();

    std::vector<std::string> getSymbol(void);


};