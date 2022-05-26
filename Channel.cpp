#include "Channel.hpp"

Channel::Channel(const std::string& name, const User& creator, const std::string& password)
: name(name), password(password), limit(0), flags(CHL_NOMSGOUT) {

	usersList.push_back(&creator);
	operatorsList.push_back(&creator);
	//sendInfo(creator);
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
	if ((flags & CHL_PRIVATE) && key != password)
		return ERR_BADCHANNELKEY;
	else if ((flags & CHL_INVITEONLY) && !isInvited(user))
		return ERR_INVITEONLYCHAN;
	else {
		for (int i = 0; i < banList.size(); i++)
			if (isBanned(user))
				return ERR_BANNEDFROMCHAN;

		std::vector<const User *>::iterator	begin = usersList.begin();
		std::vector<const User *>::iterator	end = usersList.end();
		for (; begin != end; ++begin) {
			if ((*begin)->getRealName() != user.getRealName()) {
				usersList.push_back(&user);
				//removeInvited(user);
				//sendInfo(user);
			}
		}
	}
	return 0;
}

bool Channel::isInvited(const User &user) const {

	for (int i = 0; i < inviteesList.size(); i++)
		if (inviteesList[i]->getRealName() == user.getRealName())
			return true;
	return false;
}

bool Channel::isBanned(const User &user) const {

	for (int i = 0; i < banList.size(); i++ )
		if (banList[i]->getRealName() == user.getRealName() )
			return true;
	return false;
}

bool Channel::isOperator(const User &user) const {
	for (int i = 0; i < operatorsList.size(); i++ )
		if (operatorsList[i]->getRealName() == user.getRealName() )
			return true;
	return false;
}

bool Channel::isChannelUser(const std::string &nick) const {
	for (int i = 0; i < usersList.size(); i++ )
		if (usersList[i]->getNick() == nick)
			return true;
	return false;
}

const std::string& Channel::getName() const { return name; }
unsigned char		Channel::getFlags() const { return this->flags; }

/*
void	Channel::sendMsg(const std::string &msg, const User &from, bool includeUser) const {

	std::string	message;
	message += ":" + from.getNick() + " " + msg;
	std::vector<const User *>::const_iterator	begin = usersList.begin();
	std::vector<const User *>::const_iterator	end = usersList.end();
	for (; begin != end; ++begin)
	{
		if (includeUser || *begin != &from)
			(*begin)->sendMessage(msg);
	}
}*/

void Channel::disconnect(const User &user) {

	std::vector<const User *>::iterator	begin = usersList.begin();
	std::vector<const User *>::iterator	end = usersList.end();
	for (; begin != end; ++begin)
		if (*begin == &user)
			break ;
	usersList.erase(begin);
	deleteOperator(user);
	deleteUser(user);
}

void Channel::deleteOperator(const User &user) {

	if (isOperator(user))
	{
		size_t	i;
		for (i = 0; i < operatorsList.size(); i++)
			if (operatorsList[i] == &user)
				break;
		operatorsList.erase(operatorsList.begin() + i);
		if (operatorsList.size() == 0 && usersList.size() > 0)
		{
			operatorsList.push_back(usersList[0]);
			//sendMessage("MODE " + this->name + " +o "  + usersList[0]->getNick() + "\n", user, true);
			std::cout << "MODE " + this->name + " +o "  + usersList[0]->getNick() + "\n";
		}
	}
}

void    Channel::addOperator(const User &user) {

}

void    Channel::setLimit(int limit) {
	this->limit = limit;
}

void	Channel::addBanMask(const std::string &mask) {

}

void	Channel::removeBanMask(const std::string &mask) {

}

void	Channel::setPass(const std::string) {

}

void    Channel::removePass( void ) {

}

void Channel::deleteUser(const User &user) {
	if (isChannelUser(user.getNick()))
	{
		int	i;
		for (i = 0; i < usersList.size(); i++)
			if (usersList[i] == &user)
				break;
		usersList.erase(usersList.begin() + i);
	}
}

void	Channel::setFlag(unsigned char flag)
{
	this->flags |= flag;
}

/*
		std::string					name;
		std::vector<const User *>	usersList;
		std::vector<const User *>	operatorsList;
		unsigned char			    flags;
		std::string					password;
		int		            	    limit;
		std::vector<const User *>	banList;
		std::string					topic;
		std::vector<const User *>	inviteesList;
*/

std::vector<const User *> Channel::getUsers() const { return (this->usersList); }
std::vector<const User *> Channel::getOperators() const { return (this->operatorsList); }
std::string	Channel::getPass() const { return (this->password); }
int	Channel::getLimit() const { return (this->limit); }

std::ostream	&operator<<( std::ostream &ostr, const Channel &instance )
{
	ostr    << "Channel {" << instance.getName() << "}" << std::endl;

	ostr	<< "----------" << std::endl;
	ostr	<< "\e[0;32mUSERS:\e[0m" << std::endl << std::endl;
	std::vector<const User *>::iterator i;
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
