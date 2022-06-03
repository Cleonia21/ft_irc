#include <iostream>
#include <stdio.h>

enum flags
{
	CHL_SECRET = 0b001
};


int main ()
{
	unsigned char flag = 0;
	flag = CHL_SECRET;
	flag += 32;
	std::cout << flag << std::endl;
	return (0);
}
