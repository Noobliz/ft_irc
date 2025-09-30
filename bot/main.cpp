#include "bot.hpp"

int main(int argc, char **argv)
{
    long		parsePort;
    u_int16_t	port;
    std::string	password;
    if (argc != 3)
    {
        std::cerr<<"input <port> <password>"<<std::endl;
        return 1;
    }

    password = static_cast<std::string>(argv[2]);
	parsePort = std::strtol(argv[1], NULL, 10);
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