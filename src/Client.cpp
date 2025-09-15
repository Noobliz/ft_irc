#include <Client.hpp>

Client::Client()
{

}

Client::~Client()
{

}


int		Client::getFD(void) const
{
	return _fd;
}

void	Client::setFD(int fd)
{
	_fd = fd;
}
