/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:51 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/02 02:08:47 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() : _fd(-1), _hasPass(false), _isRegistered(false) {}

Client::Client(int fd) : _fd(fd), _hasPass(false), _isRegistered(false) {}

Client::Client(const Client& other) { *this = other; }

Client& Client::operator=(const Client& other)
{
	if (this != &other) {
		_fd = other._fd;
		_buffer = other._buffer;
		_nickname = other._nickname;
		_username = other._username;
		_hostname = other._hostname;
		_servername = other._servername;
		_realname = other._realname;
		_hasPass = other._hasPass;
		_isRegistered = other._isRegistered;
	}
	return *this;
}

Client::~Client() {}

int				Client::getFd() const { return _fd; }
std::string		Client::getNickname() const { return _nickname; }
std::string		Client::getUsername() const { return _username; }
bool			Client::hasPass() const { return _hasPass; }
bool			Client::isRegistered() const { return _isRegistered; }
std::string&	Client::getBuffer() { return _buffer; }

void Client::setNickname(const std::string& nick) { _nickname = nick; }
void Client::setUsername(const std::string& user) { _username = user; }
void Client::setHostname(const std::string& host) { _hostname = host; }
void Client::setServername(const std::string& server) { _servername = server; }
void Client::setRealname(const std::string& real) { _realname = real; }
void Client::setHasPass(bool status) { _hasPass = status; }
void Client::setIsRegistered(bool status) { _isRegistered = status; }

void Client::appendBuffer(const std::string& data) { _buffer += data; }
void Client::clearBuffer() { _buffer.clear(); }