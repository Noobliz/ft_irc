#pragma once

#include <Client.hpp>
#include <iostream>
#include <map>

class Client;

class Channel
{
	public:

		Channel();
		Channel(Channel const & copy);
		Channel	&operator=(Channel const & other);
		~Channel();

		Channel(Client & client, std::string name, std::string pass);

		bool		checkPassword(std::string const & pass);

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

		void		addClient(Client & client);
		void		addOperator(Client & client);
		void		inviteClient(Client & client);

		bool		isInvited(Client & client) const;
		bool		isFull(void) const;
		bool		isOperator(Client & client) const;

		void		removeClient(std::string const & nick);
		void		removeInvited(Client & client);
		void		removeOperator(Client & client);

		std::map<std::string, Client>	&getConnectedClients();
		std::map<std::string, Client>	&getOperators();
		std::map<std::string, Client>	&getInvitedClient();

	private:

		std::string	_name;
		std::string	_topic;
		bool		_inviteMode;
		bool		_topicForAll;
		std::string	_password;
		int			_userLimit;

		std::map<std::string, Client>	_connectedClients;
		std::map<std::string, Client>	_invitedClients;
		std::map<std::string, Client>	_chanOperators;
};
