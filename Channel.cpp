#include "Channel.hpp"

Channel::Channel(const std::string& name, const User& creator, const std::string& password)
: name(name), password(password), limit(0), flags(NOMSGOUT) {

    users.push_back(&creator);
    operators.push_back(&creator);
    //sendInfo(creator);
}

Channel::~Channel() = default;

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
    if ((flags & PRIVATE) && key != password)
        return ERR_BADCHANNELKEY;
    else if ((flags & INVITEONLY) && !isInvited(user))
        return ERR_INVITEONLYCHAN;
    else {
        for (int i = 0; i < banList.size(); i++)
            if (isBanned(user))
                return ERR_BANNEDFROMCHAN;

        std::vector<const User *>::iterator	begin = users.begin();
        std::vector<const User *>::iterator	end = users.end();
        for (; begin != end; ++begin) {
            if ((*begin)->getRealName() != user.getRealName()) {
                users.push_back(&user);
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

bool Channel::isBanned(const User &user) {

    for (int i = 0; i < banList.size(); i++ )
        if (banList[i]->getRealName() == user.getRealName() )
            return true;
    return false;
}




