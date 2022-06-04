#pragma once

#include <iostream>
#include <string>
#include <vector>

class Parser
{
	std::string _delimiters;
	std::vector<std::string> _substrings;

public:
	Parser(std::string str, std::string delimiters);

	~Parser();

	std::vector<std::string> getSubstrings();

	void printSubstrings();

private:
	void erase(std::string str);
	std::vector<std::string> parseByCharacter(std::string std, char c);

};