#include "bot.hpp"

int main(int argc, char **argv)
{
	long		parsePort;
	u_int16_t	port;
	std::string	password;
	char		*end;

	if (argc != 3)
	{
		std::cerr << "input <port> <password>" << std::endl;
		return 1;
	}

	password = static_cast<std::string>(argv[2]);
	parsePort = std::strtol(argv[1], &end, 10);
	if (*end != '\0')
	{
		std::cerr << "Error: port must be an integer" << std::endl;
		return 1;
	}
	if (parsePort < 0 || parsePort > 65536)
	{
		std::cerr << "Error: port must be between 0 and 65536." << std::endl;
		return 1;
	}

	port = static_cast<u_int16_t>(parsePort);
	try {
		Bot bot(port, password);
		bot.connectToServ();
		bot.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
