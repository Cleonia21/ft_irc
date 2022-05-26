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


void	Channel::sendMsg(const std::string &msg, const User &from, bool includeUser) const
{
	std::string message;
	message += ":" + from.getNick() + " " + msg;
	std::vector<const User *>::const_iterator	begin = usersList.begin();
	std::vector<const User *>::const_iterator	end = usersList.end();
	for (; begin != end; ++begin)
	{
		if (includeUser || *begin != &from)
			(*begin)->sendMessage(message);
	}
}

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

unsigned char Channel::getFlags(void) const
{
	return (flags);
}

void	Channel::setFlag(unsigned char flag)
{
    this->flags |= flag;
}

void	Channel::clearFlag(unsigned char flag)
{
    this->flags &= ~flag;
}
