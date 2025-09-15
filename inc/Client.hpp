#pragma once

#include <includes.hpp>

class Client
{

	public :

		Client();
		~Client();

		int		getFD(void) const;
		void	setFD(int fd);

	private :

		int	_fd;

};
