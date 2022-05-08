#include "irc.hpp"
#include "Server.hpp"

int main()
{
	Server server(6667, "1111");
	
	server.execution("PASS :123 erfnjer");

	return (0);
}
