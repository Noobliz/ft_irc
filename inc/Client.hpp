#pragma once

#include <includes.hpp>
#include <Channel.hpp>
#include <map>

class Channel;

class Client
{

	public :

		Client();
		Client(int fd);
		Client(Client const & copy);
		Client	&operator=(Client const & other);
		~Client();

		int			getFD(void) const;
		void		setFD(int fd);

		std::string	getNickname(void) const;
		void		setNickname(std::string const & nickname);

		t_userinfos	getUserinfo(void) const;
		void		setUserinfo(t_userinfos const & ui);

		bool		isAuth(void) const;
		void		setAuth(bool const auth);

		bool		isInChan(std::string const & channelname) const;
		//! sert pour savoir si je peux envoyer le message dans le chan, mais aussi pour savoir si je dois effectuer ou non "join"

	private :

		int			_fd;

		//? bool		_connected;
		bool		_auth;

		//? c'est ca qui nous garanti que c'est unique.
		std::string	_nickname;

		//? on ne sait pas exactement a quoi ca sert.
		std::string	_username;
		std::string	_hostname;
		std::string	_servername;
		std::string	_realname;

		//? la liste des channels ou le client est connecté
		std::map<std::string, Channel>	_channels;

};
