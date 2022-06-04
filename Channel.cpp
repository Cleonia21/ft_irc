#include "Channel.hpp"

Channel::Channel(const std::string& name, const User& creator)
	: _name(name), _password(""), _limit(-1), _flags(CHL_NOMSGOUT) {

		
		_usersList.push_back(&creator);
		_operatorsList.push_back(&creator);
		sendJoinSuccessResponce(creator);
	}

void Channel::sendJoinSuccessResponce(const User &user) {

	user.sendMessage(":" + user.getMask() + " " + "JOIN :" + _name);
	sendNotification("JOIN :" + _name, user);
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
		if (((*itb)->getFlags() & USER_INVISIBLE) && !isChannelUser(user.getNick()))
		{
			++itb;
			continue ;
		}
		const User	*tmp = *itb;
		if (isOperator(*tmp)) // Operators
			usersList += "@";
		else if (isSpeaker(*tmp)) // Speakers
			usersList += "+";
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
	if (usersList.size())
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
	if ((_flags & CHL_PASSWORDED) && key != _password) //wrong password
		return sendServerReply(user, ERR_BADCHANNELKEY, _name);
	else if ((_flags & CHL_INVITEONLY) && !isInvited(user)) //not invited
		return sendServerReply(user, ERR_INVITEONLYCHAN, _name);
	else if (_usersList.size() >= _limit) //channel is full
		return sendServerReply(user, ERR_CHANNELISFULL, _name);
	else if (isBanned(user)) //user is banned
		return sendServerReply(user, ERR_BANNEDFROMCHAN, _name);
	else if (isChannelUser(user.getNick())) //user is already on channel
		return -1;
	
	_usersList.push_back(&user);
    deleteFromInviteesList(user);
	sendJoinSuccessResponce(user);
	return 0;
}

void Channel::deleteFromInviteesList(const User &user) {

    for (size_t i = 0; i < _inviteesList.size(); i++)
        if (_inviteesList[i] == &user)
            _inviteesList.erase(_inviteesList.begin() + i);
}

bool Channel::isInvited(const User &user) const {

	for (size_t i = 0; i < _inviteesList.size(); i++)
		if (_inviteesList[i]->getNick() == user.getNick())
			return true;
	return false;
}

bool Channel::isBanned(const User &user) const {

	for (size_t i = 0; i < _banList.size(); i++ )
		if (_banList[i]->getNick() == user.getNick() )
			return true;
	return false;
}

bool Channel::isSpeaker(const User &user) const {

	for (size_t i = 0; i < _speakersList.size(); i++ )
		if (_speakersList[i]->getNick() == user.getNick() )
			return true;
	return false;
}

bool Channel::isOperator(const User &user) const {
	for (size_t i = 0; i < _operatorsList.size(); i++ )
		if (_operatorsList[i]->getNick() == user.getNick() )
			return true;
	return false;
}

bool Channel::isChannelUser(const std::string &nick) const {
	for (size_t i = 0; i < _usersList.size(); i++ )
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
	message += ":" + user.getMask() + " " + msg;
	std::vector<const User *>::const_iterator	begin = _usersList.begin();
	std::vector<const User *>::const_iterator	end = _usersList.end();
	for (; begin != end; ++begin)// Отослать всем, кроме инициатора сообщения
		if ((*begin) != &user)
			(*begin)->sendMessage(message);
}

void Channel::disconnect(const User &user) {

	deleteUser(user);
	deleteOperator(user);
	deleteFromInviteesList(user);
	removeFromBan(user);
	removeInSpeakers(user);
}

void Channel::setTopic(const User &user, const std::string &topic) {

	if ((_flags & CHL_TOPICSET) && !isOperator(user))
		sendServerReply(user, ERR_CHANOPRIVSNEEDED, _name);
	else
	{
		this->_topic = topic;
		std::string msg = "TOPIC " + _name + " :" + this->_topic;
		user.sendMessage(":" + user.getMask() + " " + msg);
		sendNotification(msg, user);
	}
}

void Channel::fillInUsers(std::set<const User *> &uniq) const
{
	for (size_t i = 0; i < _usersList.size(); i++)
		uniq.insert(_usersList[i]);
}

void Channel::deleteOperator(const User &user) {

	if (isOperator(user)) {
		size_t i;
		for (i = 0; i < _operatorsList.size(); i++)
			if (_operatorsList[i] == &user)
				break;
		_operatorsList.erase(_operatorsList.begin() + i);
		if (_operatorsList.size() == 0 && _usersList.size() > 0)
		{
			_operatorsList.push_back(_usersList[0]);
			sendNotification("MODE " + _name + " +o :" + _usersList[0]->getNick(), user);
			std::cout << "MODE " + this->_name + " +o "  + _usersList[0]->getNick() + "\n";
		}
	}
}

void Channel::deleteUser(const User &user) {

	for (size_t i = 0; i < _usersList.size(); i++)
	{
		if (_usersList[i] == &user) {
    	    _usersList.erase(_usersList.begin() + i);
    	    break;
    	}
	}
}

void    Channel::addOperator(const User &user)
{
	if (!isOperator(user))
		_operatorsList.push_back(&user);
}

void    Channel::setLimit(int limit) {
	if (limit > 0)
		this->_limit = limit;
}

void	Channel::removeLimit(void)
{
	this->_limit = -1;
}

void	Channel::addInBan(const User &user)
{
	if (!isBanned(user))
		_banList.push_back(&user);
}

void	Channel::removeFromBan(const User &user)
{
	std::vector<const User *>::iterator result;

    result = find(_banList.begin(), _banList.end(), &user);
	if (result != _banList.end())
		_banList.erase(result);
}

void	Channel::addInSpeakers(const User &user)
{
	if (!isSpeaker(user))
		_speakersList.push_back(&user);
}

void	Channel::removeInSpeakers(const User &user)
{
	std::vector<const User *>::iterator result;

    result = find(_speakersList.begin(), _speakersList.end(), &user);
	if (result != _speakersList.end())
		_speakersList.erase(result);
}

void	Channel::setPass(const std::string pass)
{
	setFlag(CHL_PASSWORDED);
	_password = pass;
}

void    Channel::removePass( void )
{
	clearFlag(CHL_PASSWORDED);
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

void Channel::inviteToChannel(const User &user, const User &userToInvite) {

	if (_flags & CHL_INVITEONLY && !isOperator(user))
		sendServerReply(user, ERR_CHANOPRIVSNEEDED, _name);
	else
	{
		if (!isInvited(userToInvite))
			_inviteesList.push_back(&userToInvite);
		userToInvite.sendMessage(":" + user.getMask() + " INVITE " + userToInvite.getNick() + " :" + _name);
		sendServerReply(user, RPL_INVITING, _name, userToInvite.getNick());
	}
}

const std::vector<const User *> &Channel::getUsers() const { return (this->_usersList); }
const std::vector<const User *> &Channel::getOperators() const { return (this->_operatorsList); }
const std::vector<const User *> &Channel::getBans() const { return (this->_banList); }
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
