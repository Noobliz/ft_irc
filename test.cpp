#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <poll.h>
#include <vector>

#define MAX_CLIENT 3

int	main(int ac, char **av)
{
	u_int16_t	port;
	int			sockFd;
	int			clientFd;
	std::string	buff;

	if (ac == 2)
	{
		port = static_cast<u_int16_t>(std::strtol(av[1], NULL, 10));

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;          // Famille : IPv4
		addr.sin_port = htons(port);        // Port en format rÃ©seau
		addr.sin_addr.s_addr = INADDR_ANY;  // IP locale (ici toutes les interfaces)

		sockFd = socket(AF_INET, SOCK_STREAM, 0);

		fcntl(sockFd, F_SETFL, O_NONBLOCK);

		int opt = 1;
		setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		bind(sockFd, (struct sockaddr *)&addr, sizeof(addr));

		socklen_t client_len = sizeof(addr);

		listen(sockFd, MAX_CLIENT);

		struct pollfd pfd[MAX_CLIENT + 1];

		nfds_t nb_fds = 1;
		pfd[0] = {sockFd, POLLIN, 0};

		while (1)
		{
			poll(pfd, nb_fds, 0);

			if (pfd[0].revents == POLLIN)
			{
				// std::cout << "1" << std::endl;
				clientFd = accept(sockFd, (struct sockaddr *)&addr, &client_len);
				// std::cout << "2" << std::endl;
				if (clientFd != -1)
				{
					std::cout << "j'ai ecoute un client" << std::endl;
					pfd[nb_fds].fd = clientFd;
					pfd[nb_fds].events = POLLIN;
					pfd[nb_fds].revents = 0;
					nb_fds++;
				}
			}

			for (int i = 1; i < (int)nb_fds; i++)
			{
				// nfds_t nb_fds = clientFds.size();
				// poll(&(*ite), nb_fds, 0);
				
				buff = "your message longer than 10 :";
				if (pfd[i].revents == POLLIN)
				{
	
					//send(clientFd, buff.c_str(), buff.size(), 0);
					char *buffer2 = (char *)calloc(10, 1);
					recv(pfd[i].fd, buffer2, 10, 0);
					if (buffer2[0] != 0)
						std::cout << "buffer 2 = " << buffer2 << std::endl;
					free(buffer2);
					send(pfd[i].fd, (void *)"j'ai bien recu ton message merci\n\0", 10, 0);
				}
			}



		}
	}
}
