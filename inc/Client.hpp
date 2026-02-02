/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:42 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/01 23:34:29 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <poll.h>
#include <unistd.h>

class Client {
private:
	int         _fd;
	std::string _buffer;
	std::string _nickname;
	std::string _username;
	std::string _hostname;
	std::string _servername;
	std::string _realname;

	bool        _hasPass;
	bool        _isRegistered;

	public:
	Client();
	Client(int fd);
	Client(const Client& other);
	Client& operator=(const Client& other);
	~Client();

	// Getters
	int         getFd() const;
	std::string getNickname() const;
	std::string getUsername() const;
	bool        hasPass() const;
	bool        isRegistered() const;
	std::string& getBuffer();

	// Setters
	void setNickname(const std::string& nick);
	void setUsername(const std::string& user);
	void setHostname(const std::string& host);
	void setServername(const std::string& server);
	void setRealname(const std::string& real);
	void setHasPass(bool status);
	void setIsRegistered(bool status);

	void appendBuffer(const std::string& data);
	void clearBuffer();
};

#endif