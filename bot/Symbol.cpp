#include "Symbol.hpp"

Symbol::Symbol(std::vector<std::string> c)
{
    _c = c;
}

std::vector<std::string> Symbol::getSymbol(void)
{
    return (_c);
}

Symbol::~Symbol()
{

}
