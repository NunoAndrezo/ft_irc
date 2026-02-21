/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:51 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/21 15:04:40 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int clientFd, std::string serverHostname, std::string hostname, bool debug): _fd(clientFd),
							_nickname(""),
							_username(""),
							_hostname(hostname),
							_servername(""),
							_realname(""),
							_serverHostname(serverHostname),
							_hasPass(false),
							_isRegistered(false),
							_wasDisconnected(false),
							_debug(debug),
							_userMode(0){}

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
		_servername = other._servername;
		_realname = other._realname;
		_serverHostname = other._serverHostname;
		_isRegistered = other._isRegistered;
		_hasPass = other._hasPass;
		_wasDisconnected = other._wasDisconnected;
		_debug = other._debug;
		_userMode = other._userMode;
	}
	return *this;
}

void Client::reply(std::string clientCode, std::string message)
{
	// Format ":<server_hostname> <code> <nickname> :<message>\r\n"
	std::string nick = _nickname.empty() ? "unregistered " : _nickname;
	std::string replyMessage = ":" + _serverHostname + " " + clientCode + " " + nick + " " + message + CRLF;
	//debugging print
	if (_debug)
		std::cout << FBLU("[Log] Client Reply ") << replyMessage.substr(0, replyMessage.length() - 2) << std::endl; // for debugging;
	//send back to server
	send(_fd, replyMessage.c_str(), replyMessage.length(), 0);
}

int Client::getFd() const
{
	return _fd;
}

void Client::setFd(const int fd)
{
	_fd = fd;
}

std::string Client::getBuffer() const
{
	return _buffer;
}

void Client::setBuffer(const std::string& buffer)
{
	_buffer = buffer;
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

bool Client::getHasPass() const
{
	return _hasPass;
}

void Client::setHasPass(const bool status)
{
	_hasPass = status;
}

bool Client::getWasDisconnected() const
{
	return _wasDisconnected;
}

void Client::setWasDisconnected(const bool status)
{
	_wasDisconnected = status;
}

int Client::getUserMode() const
{
	return _userMode;
}

void Client::setUserMode(const int mode)
{
	_userMode |= mode;
}

void Client::appendBuffer(const std::string& data)
{
	_buffer += data;
}
void Client::clearBuffer()
{
	_buffer.clear();
}
