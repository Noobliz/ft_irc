#include <Client.hpp>

Client::Client() :
	_fd(-1),
	_nickValid(false),
	_passValid(false),
	_userValid(false),
	_auth(false)
{}
Client::Client(Client const & copy) { *this = copy; }

Client	&Client::operator=(Client const & other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_nickValid = other._nickValid;
		_passValid = other._passValid;
		_userValid = other._userValid;
		_auth = other._auth;
		_nickname = other._nickname;
		_username = other._username;
		_hostname = other._hostname;
		_servername = other._servername;
		_realname = other._realname;
		_channels = other._channels;
	}
	return *this;
}

Client::Client(int fd) :
	_fd(fd),
	_nickValid(false),
	_passValid(false),
	_userValid(false),
	_auth(false)
{

}

Client::~Client()
{
	//TODO
}


int		Client::getFD(void) const
{
	return _fd;
}

void	Client::setFD(int fd)
{
	_fd = fd;
}


std::string	Client::getNickname(void) const
{
	return _nickname;
}

void		Client::setNickname(std::string const & nickname)
{
	_nickname = nickname;
}

t_userinfos	Client::getUserinfo(void) const
{
	t_userinfos	ui;

	ui.hostname = _hostname;
	ui.realname = _realname;
	ui.servername = _servername;
	ui.username = _username;

	return ui;
}

void		Client::setUserinfo(t_userinfos const & ui)
{
	_hostname = ui.hostname;
	_realname = ui.realname;
	_servername = ui.servername;
	_username = ui.username;
}

void		Client::setNickValid(bool const nv)
{
	_nickValid = nv;
	setAuth(true);
}

void		Client::setPassValid(bool const pv)
{
	_passValid = pv;
	setAuth(true);
}

void		Client::setUserValid(bool const uv)
{
	_userValid = uv;
	setAuth(true);
}

bool		Client::isAuth(void) const
{
	return _auth;
}

void		Client::setAuth(bool const auth)
{
	if (_passValid && _nickValid && _userValid)
		_auth = auth;
}

//! A PROPOS DU const EN PARAMETRE : attention hein, si on ne trouve pas le chan alors qu'il existe, faut verif sans const
bool		Client::isInChan(std::string const & channelname) const
{
	std::map<std::string, Channel>::const_iterator	found = _channels.find(channelname);

	if (found != _channels.end())
		return true;
	else
		return false;
}

void		Client::addChan(std::string const & channelname, Channel & channel)
{
	//? channels
	_channels[channelname] = channel;
}
