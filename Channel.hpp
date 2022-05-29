#pragma once

#include <vector>
#include <string>
#include <set>
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
		std::string							_name;
		std::vector<const User *>			_usersList;
		std::vector<const User *>			_operatorsList;
		unsigned char			    		_flags;
		std::string							_password;
		int		            	    		_limit;
		std::vector<const User *>			_banList;
		std::string							_topic;
		std::vector<const User *>			_inviteesList;

	public:
		Channel(const std::string& name, const User& creator, const std::string& password = "");
		~Channel();
		int                 connect(const User &user, const std::string &key);
		static bool         isChannelNameCorrect(const std::string &name);
		bool                isInvited(const User &user) const;
		bool	        	isBanned(const User &user) const;
		bool		        isOperator(const User &user) const;
		bool		        isChannelUser(const std::string &nick) const;
		bool				isEmpty(void) const; //If channel is empty of users or not
		const std::string   &getName() const;
		void				disconnect(const User &user);
		void				deleteOperator(const User &user);
		void				deleteUser(const User &user);

		void				setFlag(unsigned char flag);
		void				clearFlag(unsigned char flag);
		void				addOperator(const User &user);
		void				setLimit(int limit);
		void				removeFromBan(const std::string &nick);
		void				addInBan(const std::string &nick);
		void				setPass(const std::string);
		void				removePass(void);
        void				sendChannelUsers(const User &user);
		void				sendChannelInfo(const User &user);
        void				sendTopic(const User &user);
        void                sendJoinSuccessResponce(const User &user);
		unsigned char		getFlags() const;
        void				setTopic(const User &user, const std::string &topic);

		//служебные для вывода
		const std::vector<const User *> &getUsers() const;
		const std::vector<const User *> &getOperators() const;
		std::string	getPass() const;
		int	getLimit() const;

		/*
		const std::string				&getName() const;
		const std::string				&getTopic() const;
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
		void							disconnect(const User &user);
		void							removeInvited(const User &user);
		void							displayChanInfo(const User &user);
		std::string						getFlagsAsString() const;
		*/
		void	            sendNotification(const std::string &msg, const User &user) const;
};

std::ostream	&operator<<( std::ostream &ostr, const Channel &instance );
