#include <Server.hpp>

int	main(int ac, char **av)
{
	long		parsePort;
	u_int16_t	port;
	char		*end;
	std::string	password;

	if (ac != 3)
	{
		std::cerr << "Error: please run this server with 2 arguments <port> <password>." << std::endl;
		return 1;
	}
	password = static_cast<std::string>(av[2]);
	parsePort = std::strtol(av[1], &end, 10);
	if (*end != '\0')
	{
		std::cerr << "Error: port must be an integer" << std::endl;
		return 1;
	}
	if (parsePort < 0 || parsePort > 65536)
	{
		std::cerr << "Error: port must be between 0 and 65536." << std::endl << "Hint: use port > 1024 unless you are root." << std::endl;
		return 1;
	}
	if (password.empty())
	{
		std::cerr << "Error: no password is highly not recommanded, please launch server with a password." << std::endl;
		return 1;
	}

	port = static_cast<u_int16_t>(parsePort);
	Server	serv(port, password);

	try
	{
		serv.init();
		serv.run();
	}
	catch (std::runtime_error &e)
	{
		return 1;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
