#include "irc.hpp"
#include "Server.hpp"
#include <map>
#include <signal.h>

void a( void ) { std::cout << "f - a" << std::endl; }
void b( void ) { std::cout << "f - b" << std::endl; }

static int activeLoop = 1;

void stopLoop(int signum)
{
	(void)signum;
	activeLoop = 0;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <pass>" << std::endl;
		return (1);
	}

	std::string port_s;
	{
		int port_i = atoi(argv[1]);
		if (port_i < 1024 || port_i > 49151) //registered ports 1024-49151
		{
			std::cout << "Incorrect port. Use 1024-49151" << std::endl;
			return (1);
		}
		std::stringstream ss;
		ss << port_i;
		port_s = ss.str();
	}

	Server server(port_s, argv[2]);
	server.socketManagement();

	signal(SIGPIPE, SIG_IGN); //prevents 'send' from causing SIGPIPE signal, if remote socket disconnected
	signal(SIGINT, stopLoop);
	while (activeLoop)
	{
		server.acceptConnection();

		server.disconnectUsers();
	}
	return (0);
}
