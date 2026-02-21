/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:42 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/21 15:05:31 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>

#include "../utils/IRCmacros.hpp"
#include "../utils/stdoutColors.hpp"

#define USER_MODE_INVISIBLE 0x0001

// Example
// USER	<username>	<hostname>	<servername>	:<realname>
// USER	etom		IP do user	IP do servidor		:etom

class Client {
	private:

		int			_fd;
		std::string	_buffer;
	
		std::string	_nickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_servername;
		std::string	_realname;

		std::string	_serverHostname;

		bool		_hasPass;
		bool		_isRegistered;

		bool		_wasDisconnected;
		bool		_debug;

		int			_userMode; // 0x0001 invisible, 0x0010 operator.

	public:

		Client();
		Client(int fd, std::string serverHostname, std::string hostname, bool debug);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);

		void reply(std::string clientCode, std::string message);

		// Getters and Setters
		int getFd() const;
		void setFd(const int fd);

		std::string getBuffer() const;
		void setBuffer(const std::string& buffer);
	
		std::string getNickname() const;
		void setNickname(const std::string& nickname);

		std::string getUsername() const;
		void setUsername(const std::string& username);

		std::string getHostname() const;
		void setHostname(const std::string& hostname);

		std::string getRealname() const;
		void setRealname(const std::string& realname);

		std::string getServername() const;
		void setServername(const std::string& servername);

		bool getIsRegistered() const;
		void setIsRegistered(const bool status);
	
		bool getHasPass() const;
		void setHasPass(const bool status);

		bool getWasDisconnected() const;
		void setWasDisconnected(const bool status);

		int getUserMode() const;
		void setUserMode(const int mode);		

		// Buffer Methods
		void appendBuffer(const std::string& data);
		void clearBuffer();

};

#endif