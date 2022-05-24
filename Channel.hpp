#pragma once

#include <vector>
#include <string>
#include "irc.hpp"
#include "User.hpp"

enum Channel_states {
    PRIVATE	=       0b000001,
    SECRET =        0b000010,
    MODERATED =     0b000100,
    INVITEONLY =    0b001000,
    TOPICSET =      0b010000,
    NOMSGOUT =      0b100000
};

class Channel
{
	private:
		std::string					name;
		std::vector<const User *>	users;
		std::vector<const User *>	operators;
        char					    flags;
        std::string					password;
        int		            	    limit;
        std::vector<std::string>	bans;
        std::string					topic;
        std::vector<const User *>	invitedUsers;

	public:
		Channel(std::string &channel_name, const User &creator, const std::string &password = "");
        ~Channel();
        int connect(const User &user, const std::string &key);
        static bool isChannelNameCorrect(const std::string &name);
        /*
        const std::string				&getName() const;
        const std::string				&getTopic() const;
        unsigned char					getFlags() const;
        void							setTopic(const User &user, const std::string &topic);
        void							setLimit(unsigned short limit);
        void							setKey(const User &user, const std::string &key);
        bool							isInvited(const User &user) const;
        bool							isOperator(const User &user) const;
        bool							isSpeaker(const User &user) const;
        bool							isEmpty() const;
        bool							containsNickname(const std::string &nickname) const;
        void							setFlag(unsigned char flag);
        void							removeFlag(unsigned char flag);
        void							sendMessage(const std::string &message, const User &from, bool includeUser) const;
        void							invite(const User &user, const User &receiver);
        void							addOperator(const User &user);
        void							removeOperator(const User &user);
        void							addSpeaker(const User &user);
        void							removeSpeaker(const User &user);
        void							addBanMask(const std::string &mask);
        void							removeBanMask(const std::string &mask);
        void							displayTopic(const User &user);
        void							displayNames(const User &user);
        void							disconnect(const User &user);
        void							removeInvited(const User &user);
        void							displayChanInfo(const User &user);
        std::string						getFlagsAsString() const;
        */
};
