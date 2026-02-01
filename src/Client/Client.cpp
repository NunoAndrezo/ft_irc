/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:51 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/01 19:27:49 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientFd) : _fd(clientFd),
								_isRegistered(false),
								_isAuthenticated(false),
								_nickname(""),
								_username(""),
								_hostname(""),
								_realname(""),
								_servername("")
{

}

Client::~Client()
{
	if (_fd != -1)
		close(_fd);
}

Client::Client(const Client& other)
{
	*this = other;
}

Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_nickname = other._nickname;
		_username = other._username;
		_hostname = other._hostname;
		_realname = other._realname;
		_servername = other._servername;
		_isRegistered = other._isRegistered;
		_isAuthenticated = other._isAuthenticated;
	}
	return *this;
}

int Client::getFd() const
{
	return _fd;
}

void Client::setFd(const int fd)
{
	_fd = fd;
}

std::string Client::getNickname() const
{
	return _nickname;
}

void Client::setNickname(const std::string& nickname)
{
	_nickname = nickname;
}

std::string Client::getUsername() const
{
	return _username;
}

void Client::setUsername(const std::string& username)
{
	_username = username;
}

std::string Client::getHostname() const
{
	return _hostname;
}

void Client::setHostname(const std::string& hostname)
{
	_hostname = hostname;
}

std::string Client::getRealname() const
{
	return _realname;
}

void Client::setRealname(const std::string& realname)
{
	_realname = realname;
}

std::string Client::getServername() const
{
	return _servername;
}

void Client::setServername(const std::string& servername)
{
	_servername = servername;
}

bool Client::getIsRegistered() const
{
	return _isRegistered;
}

void Client::setIsRegistered(const bool status)
{
	_isRegistered = status;
}

bool Client::getIsAuthenticated() const
{
	return _isAuthenticated;
}

void Client::setIsAuthenticated(const bool status)
{
	_isAuthenticated = status;
}