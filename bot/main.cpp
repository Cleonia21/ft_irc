#include "Bot.hpp"
#include <signal.h>


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
		std::cout << "Usage: ./mebot <port> <password>" << std::endl;
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

	Bot mebot(port_s, argv[2]);
	mebot.socketManagement();
	mebot.authorize();

	signal(SIGINT, stopLoop);
	signal(SIGPIPE, SIG_IGN); //prevents 'send' from causing SIGPIPE signal, if remote socket disconnected
	while (activeLoop)
	{
		mebot.checkForData();
	}
	return (0);
}
