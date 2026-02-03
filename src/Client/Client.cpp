/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:51 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/03 13:26:05 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* Client::Client(): _fd(-1),
				_nickname(""),
				_username(""),
				_hostname(""),
				_servername(""),
				_realname(""),
				_hasPass(false),
				_isRegistered(false)
				_
{

} */

Client::Client(int clientFd, std::string serverHostname): _fd(clientFd),
							_nickname(""),
							_username(""),
							_hostname(""),
							_servername(""),
							_realname(""),
							_serverHostname(serverHostname),
							_hasPass(false),
							_isRegistered(false),
							_wasDisconnected(false)
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
		_hasPass = other._hasPass;
	}
	return *this;
}

void Client::reply(std::string clientCode, std::string message) // example: ": " + servername +  + ":" + "possible message" + "\r\n";
{
	std::string nick = _nickname.empty() ? "unregistered " : _nickname;
	std::string replyMessage = ":" + _serverHostname + " " + clientCode + " " + nick + " " + message + CRLF;

	std::cout << "Client Reply: " << replyMessage << std::endl; // for debugging;
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

void Client::appendBuffer(const std::string& data)
{
	_buffer += data;
}
void Client::clearBuffer()
{
	_buffer.clear();
}
