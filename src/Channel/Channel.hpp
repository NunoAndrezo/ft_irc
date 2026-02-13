/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:37:39 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/13 13:41:17 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "../Client/Client.hpp"
#include "../Server/Server.hpp"

#define MD_INV		0x0001 // invite only
#define MD_TOPIC	0x0002 // topic command to
#define MD_PASSWORD_PROT	0x0004 // password flag
#define MD_USR_LIM		0x0008 // user limit flag

class Server;

class Channel
{
	private:
		std::string const _name;
		std::string _topic;
		std::string _password;
		std::map<Client*, bool> _members; // bool indicates if member is operator
		std::vector<std::string> _inviteList;
		int _userLimit; // only relevant if MD_USR_LIM is set

		int _modes; // bitmask for channel modes
		bool _debug;

	public:
		Channel(std::string name, bool _debug);
		~Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

		std::string getName() const;

		std::string getTopic() const;
		void setTopic(const std::string &topic);

		std::string getPassword() const;
		void setPassword(const std::string &password);

		int getUserLimit() const;
		void setUserLimit(int limit);

		int getModes() const;
		void setModes(int modes);
		void addMode(int mode);
		void removeMode(int mode);

		std::map<Client*, bool> getMembers() const;
		void addMember(Client *client);
		void removeMember(Client *client);
		Client *findMember(Client *client) const;
		void setMemberAsOperator(Client *client);
		void unsetMemberAsOperator(Client *client);
		bool isMemberOperator(Client *client) const;
		int getOperatorsCount() const;

		std::vector<std::string> getInviteList() const;
		void addToInviteList(const std::string &nickname);
		void removeFromInviteList(const std::string &nickname);
		bool isInInviteList(const std::string &nickname) const;

		void broadcastMessage(const std::string &message, Client *sender) const;
		void broadcastRawMessage(const std::string &message) const;
};


#endif