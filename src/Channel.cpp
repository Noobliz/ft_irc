#include <Channel.hpp>

Channel::Channel() {}
Channel::Channel(Channel const & copy) { *this = copy; }
Channel	&Channel::operator=(Channel const & other)
{
	if (this != &other)
	{
		_name = other._name;
		_inviteMode = other._inviteMode;
		_password = other._password;
		_userLimit = other._userLimit;
		_connectedClients = other._connectedClients;
		_invitedClients = other._invitedClients;
		_chanOperators = other._chanOperators;
	}
	return *this;
}

Channel::~Channel()
{
	//TODO
}

Channel::Channel(Client & client, std::string name, std::string pass) :
	_name(name),
	_password(pass)
{
	addOperator(client);
}

bool		Channel::checkPassword(std::string pass)
{
	if (_password == pass)
		return true;
	return false;
}

bool		Channel::isInInviteMode(void)
{
	return _inviteMode;
}
void		Channel::setInviteMode(bool const & im)
{
	_inviteMode = im;
}

bool		Channel::isTopicForAll(void)
{
	return _topicForAll;
}

void		Channel::setTopicForAll(bool const & tfa)
{
	_topicForAll = tfa;
}

std::string	Channel::getName(void) const
{
	return _name;
}
void		Channel::setName(std::string const & name)
{
	_name = name;
}

std::string	Channel::getTopic(void) const
{
	return _topic;
}

void		Channel::setTopic(std::string const & topic)
{
	_topic = topic;
}

std::string	Channel::getPassword(void) const
{
	return _password;
}

void		Channel::setPassword(std::string const & pass)
{
	_password = pass;
}

int			Channel::getUserLimit(void) const
{
	return _userLimit;
}

void		Channel::setUserLimit(int const & ul)
{
	_userLimit = ul;
}

//? j'ai besoin pour join, d'un addclient pour chacune des maps string/Client
void		Channel::addClient(Client & client)
{
	std::map<std::string, Client>::const_iterator	inviteIter = _invitedClients.find(client.getNickname());
	std::map<std::string, Client>::const_iterator	channelIter = _connectedClients.find(client.getNickname());

	if ((inviteIter != _invitedClients.end() || _inviteMode == false) && channelIter == _connectedClients.end())
		_connectedClients[client.getNickname()] = client;
	else
	{
		//? message d'erreur ?????????? feedback sur server ??????
	}
} //! attention: verifier si invité.

void		Channel::addOperator(Client & client)
{
	std::map<std::string, Client>::const_iterator	operIter = _chanOperators.find(client.getNickname());

	if (operIter == _chanOperators.end())
		_chanOperators[client.getNickname()] = client;
}

void		Channel::inviteClient(Client & client)
{
	std::map<std::string, Client>::const_iterator	inviteIter = _invitedClients.find(client.getNickname());

	if (inviteIter == _invitedClients.end() && _inviteMode == true)
		_invitedClients[client.getNickname()] = client;
}

std::map<std::string, Client>	&Channel::getConnectedClients()
{
	return _connectedClients;
}
