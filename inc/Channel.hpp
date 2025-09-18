#pragma once

#include <includes.hpp>
#include <Client.hpp>
#include <map>

class Client;

class Channel
{
	public:

		Channel(Channel const & copy);
		Channel	&operator=(Channel const & other);
		~Channel();

		//? attention, le client qui construit la class Channel doit etre operateur
		Channel(Client & client, std::string name, std::string pass);

		bool		checkPassword(std::string pass);

		bool		isInInviteMode(void);
		void		setInviteMode(bool const & im);

		bool		isTopicForAll(void);
		void		setTopicForAll(bool const & tfa);

		std::string	getName(void) const;
		void		setName(std::string const & name);

		std::string	getTopic(void) const;
		void		setTopic(std::string const & topic);

		std::string	getPassword(void) const;
		void		setPassword(std::string const & pass);

		int			getUserLimit(void) const;
		void		setUserLimit(int const & ul);

		std::map<std::string, Client>	getConnectedClients();
		//? j'ai besoin pour join, d'un addclient pour chacune des maps string/Client
		void		addClient(Client & client); //! attention: verifier si invité.
		void		addOperator(Client & client);
		void		inviteClient(Client & client);

	private:

		Channel();

		std::string						_name;
		std::string						_topic;
		bool							_inviteMode;
		bool							_topicForAll;
		std::string						_password;
		int								_userLimit;

		//? liste des clients connectés
		std::map<std::string, Client>	_connectedClients;

		//? liste des clients invités
		std::map<std::string, Client>	_invitedClients;

		//? liste des clients operateurs
		std::map<std::string, Client>	_chanOperators;


};
