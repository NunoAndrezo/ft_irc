/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:42 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/03 21:46:31 by marvin           ###   ########.fr       */
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

// USER	<username>	<hostname>	<servername>	:<realname>
// USER	etom		IP do user	IP do servidor		:etom



// int result = getnameinfo((struct sockaddr *)&client_address, client_address_size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);

class Client {
	private:

		int			_fd;
		std::string	_buffer; // tentar usar class mesagem como buffer
	
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
		

		// Buffer Methods

		void appendBuffer(const std::string& data);
		void clearBuffer();

};

#endif