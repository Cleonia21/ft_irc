#include "Channel.hpp"

Channel::Channel(const std::string& name, const User& creator, const std::string& password)
	: _name(name), _password(password), _limit(0), _flags(CHL_NOMSGOUT) {

		_usersList.push_back(&creator);
		_operatorsList.push_back(&creator);

		sendJoinSuccessResponce(creator);
	}

void Channel::sendJoinSuccessResponce(const User &user) {

	user.sendMessage(":" + user.getMask() + " " + "JOIN :" + _name + "\n");
	if (!_topic.empty())
		sendTopic(user);
	sendChannelUsers(user);
	sendServerReply(user, RPL_ENDOFNAMES, _name);
}

void Channel::sendTopic(const User &user) {

	if (_topic.empty())
		sendServerReply(user, RPL_NOTOPIC, _name);
	else
		sendServerReply(user, RPL_TOPIC, _name, _topic);
}

void	Channel::sendChannelUsers(const User &user) {

	std::string	usersList;
	std::vector<const User *>::const_iterator	itb = _usersList.begin();
	std::vector<const User *>::const_iterator	ite = _usersList.end();
	while (itb != ite)
	{
		if ((*itb)->getFlags() & USER_INVISIBLE)
		{
			++itb;
			continue ;
		}
		const User	*tmp = *itb;
		if (isOperator(*tmp))
			usersList += "@";
//		else if (isChannelUser((*tmp).getNick())) // '+' только для спикеров, которых пока нет
//			usersList += "+";
		usersList += tmp->getNick();
		++itb;
		if (itb != ite)
			usersList += " ";
	}
	std::string prefixToName = "= ";
	if (_flags & CHL_SECRET)
		prefixToName = "@ ";
	else if (_flags & CHL_PRIVATE)
		prefixToName = "* ";
	sendServerReply(user, RPL_NAMREPLY, prefixToName + _name, usersList);
}

void	Channel::sendChannelInfo(const User &user)
{
	std::string channelName; //Channel name
	std::stringstream ss; //Number of users
	ss << _usersList.size();
	if ((_flags & CHL_SECRET) && !isChannelUser(user.getNick()))
		return ;
	if ((_flags & CHL_PRIVATE) && !isChannelUser(user.getNick()))
		channelName = "Prv";
	else
		channelName = _name;
	sendServerReply(user, RPL_LIST, channelName, ss.str(), _topic);
}

Channel::~Channel() {};

bool Channel::isChannelNameCorrect(const std::string &name) {

	if (name[0] != '#' && name[0] != '&')
		return false;
	for (size_t i = 1; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == 7 || name[i] == 0 \
				|| name[i] == 13 || name[i] == 10 || name[i] == ',')
			return false;
	}

	return true;
}

int	Channel::connect(const User &user, const std::string &key) {
	if ((_flags & CHL_PRIVATE) && key != _password)
		return ERR_BADCHANNELKEY;
	else if ((_flags & CHL_INVITEONLY) && !isInvited(user))
		return ERR_INVITEONLYCHAN;
	else {
		for (int i = 0; i < _banList.size(); i++)
			if (isBanned(user))
				return ERR_BANNEDFROMCHAN;

		std::vector<const User *>::iterator	begin = _usersList.begin();
		std::vector<const User *>::iterator	end = _usersList.end();
		for (; begin != end; ++begin) {
			if ((*begin)->getNick() != user.getNick()) {
				_usersList.push_back(&user);
				//removeInvited(user);
				sendJoinSuccessResponce(user);
			}
		}
	}
	return 0;
}

bool Channel::isInvited(const User &user) const {

	for (int i = 0; i < _inviteesList.size(); i++)
		if (_inviteesList[i]->getNick() == user.getNick())
			return true;
	return false;
}

bool Channel::isBanned(const User &user) const {

	for (int i = 0; i < _banList.size(); i++ )
		if (_banList[i]->getNick() == user.getNick() )
			return true;
	return false;
}

bool Channel::isOperator(const User &user) const {
	for (int i = 0; i < _operatorsList.size(); i++ )
		if (_operatorsList[i]->getNick() == user.getNick() )
			return true;
	return false;
}

bool Channel::isChannelUser(const std::string &nick) const {
	for (int i = 0; i < _usersList.size(); i++ )
		if (_usersList[i]->getNick() == nick)
			return true;
	return false;
}

bool Channel::isEmpty(void) const
{
	if (_usersList.size())
		return (false);
	return (true);
}

const std::string& Channel::getName() const { return _name; }


void	Channel::sendNotification(const std::string &msg, const User &user) const
{
	std::string message;
	message += ":" + user.getMask() + " " + msg + "\n";
	std::vector<const User *>::const_iterator	begin = _usersList.begin();
	std::vector<const User *>::const_iterator	end = _usersList.end();
	for (; begin != end; ++begin)
		(*begin)->sendMessage(message);
}

void Channel::disconnect(const User &user) {

	std::vector<const User *>::iterator	begin = _usersList.begin();
	std::vector<const User *>::iterator	end = _usersList.end();
	for (; begin != end; ++begin)
		if (*begin == &user)
			break ;
	_usersList.erase(begin);
	deleteOperator(user);
	deleteUser(user);
}

void Channel::setTopic(const User &user, const std::string &topic) {

	if ((_flags & CHL_TOPICSET) && !isOperator(user))
		sendServerReply(user, ERR_CHANOPRIVSNEEDED, _name);
	else
	{
		this->_topic = topic;
		sendNotification("TOPIC " + _name + " :" + this->_topic, user);
	}
}

void Channel::deleteOperator(const User &user) {

	if (isOperator(user))
	{
		size_t	i;
		for (i = 0; i < _operatorsList.size(); i++)
			if (_operatorsList[i] == &user)
				break;
		_operatorsList.erase(_operatorsList.begin() + i);
		if (_operatorsList.size() == 0 && _usersList.size() > 0)
		{
			_operatorsList.push_back(_usersList[0]);
			//sendMessage("MODE " + this->name + " +o "  + usersList[0]->getNick() + "\n", user, true);
			std::cout << "MODE " + this->_name + " +o "  + _usersList[0]->getNick() + "\n";
		}
	}
}

void Channel::deleteUser(const User &user) {
	if (isChannelUser(user.getNick()))
	{
		size_t i;
		for (i = 0; i < _usersList.size(); i++)
			if (_usersList[i] == &user)
				break;
		_usersList.erase(_usersList.begin() + i);
	}
}

void    Channel::addOperator(const User &user) 
{
	_operatorsList.push_back(&user);
}

void    Channel::setLimit(int limit) {
	this->_limit = limit;
}

void	Channel::addInBan(const std::string &nick)
{

}

void	Channel::removeFromBan(const std::string &nick)
{

}

void	Channel::setPass(const std::string pass)
{
	_password = pass;
}

void    Channel::removePass( void )
{
	_password.clear();
}

unsigned char Channel::getFlags(void) const
{
	return (_flags);
}

void	Channel::setFlag(unsigned char flag)
{
	this->_flags |= flag;
}

void	Channel::clearFlag(unsigned char flag)
{
	this->_flags &= ~flag;
}

const std::vector<const User *> &Channel::getUsers() const { return (this->_usersList); }
const std::vector<const User *> &Channel::getOperators() const { return (this->_operatorsList); }
std::string	Channel::getPass() const { return (this->_password); }
int	Channel::getLimit() const { return (this->_limit); }

std::ostream	&operator<<( std::ostream &ostr, const Channel &instance )
{
	ostr    << "Channel {" << instance.getName() << "}" << std::endl;

	ostr	<< "----------" << std::endl;
	ostr	<< "\e[0;32mUSERS:\e[0m" << std::endl << std::endl;
	std::vector<const User *>::const_iterator i;
	for (i = instance.getUsers().begin(); i != instance.getUsers().end(); i++)
		ostr << **i << std::endl << std::endl;
	ostr	<< "----------" << std::endl;

	ostr	<< "\e[0;32mOPERATORS:\e[0m" << std::endl << std::endl;
	for (i = instance.getOperators().begin(); i != instance.getOperators().end(); i++)
		ostr << **i << std::endl << std::endl;
	ostr	<< "----------" << std::endl;

	unsigned char flags = instance.getFlags();
	ostr	<< "\e[0;32mFLAGS:\e[0m" << std::endl
		<< "\e[0;33mprivate:    \e[0m" << (bool)(flags & 0b000001) << std::endl
		<< "\e[0;33msecret:     \e[0m" << (bool)(flags & 0b000010) << std::endl
		<< "\e[0;33mmoderated:  \e[0m" << (bool)(flags & 0b000100) << std::endl
		<< "\e[0;33minv only:   \e[0m" << (bool)(flags & 0b001000) << std::endl
		<< "\e[0;33mtopic set:  \e[0m" << (bool)(flags & 0b010000) << std::endl
		<< "\e[0;33mno msg out: \e[0m" << (bool)(flags & 0b100000) << std::endl;
	ostr	<< "----------" << std::endl;

	ostr	<< "\e[0;32mPASSWORD: \e[0m" << instance.getPass() << std::endl;
	ostr	<< "\e[0;32mLIMIT:    \e[0m" << instance.getLimit() << std::endl;
	ostr	<< "----------" << std::endl;

	return (ostr);
}

/*
   CHL_PRIVATE	=       0b000001,
   CHL_SECRET =        0b000010,
   CHL_MODERATED =     0b000100,
   CHL_INVITEONLY =    0b001000,
   CHL_TOPICSET =      0b010000,
   CHL_NOMSGOUT =      0b100000
   */
