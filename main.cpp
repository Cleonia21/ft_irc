#include "irc.hpp"
#include "Server.hpp"
#include <map>

void a( void ) { std::cout << "f - a" << std::endl; }
void b( void ) { std::cout << "f - b" << std::endl; }

int main(int argc, char **argv)
{
	Server server(6667, "1111");
	
	if (argc > 1)
		server.execution(argv[1]);
	else
		server.execution("PASS :123 erfnjer");

	return (0);
}
