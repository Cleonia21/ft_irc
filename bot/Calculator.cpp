#include "Calculator.hpp"

Calculator::Calculator(std::string str)
{
	std::cout << "log:str {" + str + "}" << std::endl;
	std::vector<std::string> substrings = Parser(str, " -+*/").getSubstrings();

	if (!validityCheck(substrings))
		_result = "Error: bad suntax: use double nums and +,-,/,*";
	else
	{
		calculation(substrings, "*/");
		calculation(substrings, "+-");
		_result = substrings[0];
	}
}

void Calculator::calculation(std::vector<std::string> &substrings, std::string signs)
{
	std::vector<std::string>::iterator substr;
	double num1, num2, buf;
	for (substr = substrings.begin(); substr != substrings.end(); substr++)
	{
		if (searchSumbolsInString(*substr, signs))
		{
			num1 = strtod((*(substr - 1)).c_str(), NULL);
			num2 = strtod((*(substr + 1)).c_str(), NULL);
			if (*substr == "*")
				buf = num1 * num2;
			if (*substr == "/")
				buf = num1 / num2;
			if (*substr == "+")
				buf = num1 + num2;
			if (*substr == "-")
				buf = num1 - num2;
			substr = substrings.erase(substr - 1, substr + 2);
			substrings.insert(substr, std::to_string(buf));
		}
	}
}

bool Calculator::validityCheck(std::vector<std::string> substrings)
{
	bool flag = true;

	for (size_t i = 0; i < substrings.size(); i++)
	{
		if (flag)
		{
			if (!checkToDouble(substrings[i]))
				return false;
		}
		else
		{
			if (substrings[i] != "+" && substrings[i] != "-"
				&& substrings[i] != "*" && substrings[i] != "/")
				return false;
		}
		flag = !flag;
	}
	if (flag)
		return false;
	return true;
}

bool    Calculator::checkToDouble(std::string str)
{
    bool pointFlag = false;

    for (size_t i = 0; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
            continue;
        if (str[i] == '.' && !pointFlag)
        {
            pointFlag = true;
            continue;
        }
        return false;
    }
    return true;
}

std::string Calculator::getResult()
{
	return (_result);
}

bool    Calculator::searchSumbolsInString(std::string str1, std::string str2)
{
	for (size_t i = 0; i < str1.size(); i++)
	{
		for (size_t j = 0; j < str2.size(); j++)
		{
			if (str1[i] == str2[j])
				return true;
		}
	}
	return false;
}

// int main()
// {
// 	Calculator calculator("1+2*3 + 12.3243423 / 3");
// 	std::cout << calculator.getResult() << std::endl;
// }