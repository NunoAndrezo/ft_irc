/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 14:38:09 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/21 15:03:57 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, bool debug): _name(name),
													_topic(" "),
													_password(""),
													_userLimit(0),
													_modes(0),
													_debug(debug)
{

}

Channel::~Channel(){
}

Channel::Channel(const Channel &other): _name(other._name),
										_topic(other._topic),
										_password(other._password),
										_members(other._members),
										_userLimit(other._userLimit),
										_modes(other._modes),
										_debug(other._debug)

{

}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{
		// _name is const, cannot be assigned
		_topic = other._topic;
		_password = other._password;
		_members = other._members;
		_userLimit = other._userLimit;
		_modes = other._modes;
		_debug = other._debug;
	}
	return *this;
}

std::string Channel::getName() const
{
	return _name;
}

std::string Channel::getTopic() const
{
	return _topic;
}

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

std::string Channel::getPassword() const
{
	return _password;
}

void Channel::setPassword(const std::string &password)
{
	_password = password;
}

int Channel::getUserLimit() const
{
	return _userLimit;
}

void Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}

int Channel::getModes() const
{
	return _modes;
}

void Channel::setModes(int modes)
{
	_modes = modes;
}

void Channel::addMode(int mode) // set bit on the left to the one on the right
{
	_modes |= mode;
}

void Channel::removeMode(int mode) // set bit on the left to 0 where there's a 1 on the right (and vice versa)
{
	_modes &= ~mode;
}

std::map<Client*, bool> Channel::getMembers() const
{
	return _members;
}

void Channel::addMember(Client *client)
{
	_members[client] = false; // not operator by default
}

void Channel::removeMember(Client *client)
{
	_members.erase(client);
}

Client *Channel::findMember(Client *client) const
{
	std::map<Client*, bool>::const_iterator it = _members.find(client);
	return it != _members.end() ? it->first : NULL;
}

void Channel::setMemberAsOperator(Client *client)
{
	std::map<Client*, bool>::iterator it = _members.find(client);
	if (it != _members.end())
		it->second = true;
}

void Channel::unsetMemberAsOperator(Client *client)
{
	std::map<Client*, bool>::iterator it = _members.find(client);
	if (it != _members.end())
		it->second = false;
}

bool Channel::isMemberOperator(Client *client) const
{
	std::map<Client*, bool>::const_iterator it = _members.find(client);
	return it != _members.end() ? it->second : false;
}

int Channel::getOperatorsCount() const
{
	int count = 0;
	for (std::map<Client*, bool>::const_iterator it = _members.begin(); it != _members.end(); ++it)
	{
		if (it->second) // if member is operator
			count++;
	}
	return count;
}

std::vector<std::string> Channel::getInviteList() const
{
	return _inviteList;
}

void Channel::addToInviteList(const std::string &nickname)
{
	if (!Channel::isInInviteList(nickname))
		_inviteList.push_back(nickname);
}

void Channel::removeFromInviteList(const std::string &nickname)
{
	std::vector<std::string>::iterator it = std::find(_inviteList.begin(), _inviteList.end(), nickname);
	if (it != _inviteList.end())
		_inviteList.erase(it);
}

bool Channel::isInInviteList(const std::string &nickname) const
{
	return std::find(_inviteList.begin(), _inviteList.end(), nickname) != _inviteList.end();
}

void Channel::broadcastMessage(const std::string &message, Client *sender) const
{
	// Criamos o prefixo completo uma única vez fora do loop para ser mais eficiente
	// Formato: :Nick!User@Host PRIVMSG #canal :mensagem\r\n
	std::string fullMsg = ":" + sender->getNickname() + "!" + sender->getUsername() + "@" + sender->getHostname() 
						+ " PRIVMSG " + _name + " " + message + "\r\n";

	for (std::map<Client*, bool>::const_iterator it = _members.begin(); it != _members.end(); ++it)
	{
		Client* member = it->first;
		if (member != sender) // Comparar ponteiros é mais rápido que comparar strings de nicknames
		{
			send(member->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
		}
	}
}

void Channel::broadcastRawMessage(const std::string &message) const
{
	for (std::map<Client*, bool>::const_iterator it = _members.begin(); it != _members.end(); ++it)
	{
		send(it->first->getFd(), message.c_str(), message.length(), 0);
	}
}

std::string Channel::getNamesList() const {
	std::string list = "";
	for (std::map<Client*, bool>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		if (!list.empty()) list += " ";
		if (it->second) // If is operator
			list += "@";
		list += it->first->getNickname();
	}
	return list;
}

size_t Channel::getMemberCount() const {
	return _members.size();
}

void Channel::broadcast(std::string message) {
	if (message.find("\r\n") == std::string::npos)
		message += "\r\n";
		
	for (std::map<Client*, bool>::iterator it = _members.begin(); it != _members.end(); ++it) {
		// it->first é o ponteiro Client*
		send(it->first->getFd(), message.c_str(), message.length(), 0);
	}
}

bool Channel::isMember(Client *c)
{
	// O find procura pela chave (ponteiro do Client)
	std::map<Client*, bool>::const_iterator it = _members.find(c);
	// Se não chegou ao fim, o gajo está no mapa
	return it != _members.end();
}
