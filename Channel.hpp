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
		std::string					_name;
		std::vector<const User *>	_usersList;
		std::vector<const User *>	_operatorsList;
		unsigned char			    _flags;
		std::string					_password;
		int		            	    _limit;
		std::vector<const User *>	_banList;
		std::string					_topic;
		std::vector<const User *>	_inviteesList;

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
        void				sendChannelUsers(const User &user);
        void				sendTopic(const User &user);
        void                sendJoinSuccessResponce(const User &user);
		unsigned char					getFlags() const;
        void	            sendNotification(const std::string &msg, const User &user) const;

};
