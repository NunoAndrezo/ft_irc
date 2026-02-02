/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:42 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/02 12:53:46 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "../utils/IRCmacros.hpp"

// USER	<username>	<hostname>	<servername>	:<realname>
// USER	etom		etom		0.0.0.0			:etom

class Client {
	private:
		std::string _serverHostname;

		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_realname;
		std::string _servername;

		bool		_isRegistered;
		bool		_isAuthenticated;

	public:

		Client(int clientFd, std::string serverHostname);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);

		void reply(std::string clientCode, std::string message);

		int getFd() const;
		void setFd(const int fd);
	
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
	
		bool getIsAuthenticated() const;
		void setIsAuthenticated(const bool status);

};

#endif