#pragma once

#include <vector>
#include <string>
#include "irc.hpp"
#include "User.hpp"

enum Channel_states {
	CHL_PRIVATE	=       0b000001,
	CHL_SECRET =        0b000010,
	CHL_MODERATED =     0b000100,
	CHL_INVITEONLY =    0b001000,
	CHL_TOPICSET =      0b010000,
	CHL_NOMSGOUT =      0b100000
};

class User;

class Channel
{
	private:
		std::string					name;
		std::vector<const User *>	usersList;
		std::vector<const User *>	operatorsList;
		unsigned char			    flags;
		std::string					password;
		int		            	    limit;
		std::vector<const User *>	banList;
		std::string					topic;
		std::vector<const User *>	inviteesList;

	public:
		Channel(const std::string& name, const User& creator, const std::string& password = "");
		~Channel();
		int                 connect(const User &user, const std::string &key);
		static bool         isChannelNameCorrect(const std::string &name);
		bool                isInvited(const User &user) const;
		bool	        	isBanned(const User &user) const;
		bool		        isOperator(const User &user) const;
		bool		        isChannelUser(const std::string &nick) const;
		const std::string   &getName() const;
		void				sendMsg(const std::string &msg, const User &from, bool includeUser) const;
		void				disconnect(const User &user);
		void				deleteOperator(const User &user);
		void				deleteUser(const User &user);

		void				setFlag(unsigned char flag);
		void				clearFlag(unsigned char flag);
		void				addOperator(const User &user);
		void				setLimit(int limit);
		void				addBanMask(const std::string &mask);
		void				removeBanMask(const std::string &mask);
		void				setPass(const std::string);
		void				removePass(void);

		unsigned char					getFlags() const;

		/*
		const std::string				&getName() const;
		const std::string				&getTopic() const;
		void							setTopic(const User &user, const std::string &topic);
		void							setKey(const User &user, const std::string &key);

		bool							isOperator(const User &user) const;
		bool							isSpeaker(const User &user) const;
		bool							isEmpty() const;
		bool							containsNickname(const std::string &nickname) const;
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
