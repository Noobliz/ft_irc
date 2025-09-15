#include <includes.hpp>
#include <Server.hpp>

int	main(int ac, char **av)
{
	u_int16_t	port;

	if (ac == 2)
	{
		port = static_cast<u_int16_t>(std::strtol(av[1], NULL, 10));
		Server serv(port);

		try
		{
			serv.init();
			serv.run();
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
}
