/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:42 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/01 19:27:55 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <poll.h>
#include <unistd.h>

// USER	<username>	<hostname>	<servername>	:<realname>
// USER	etom		etom		0.0.0.0			:etom

class Client { // int result = getnameinfo((struct sockaddr *)&client_address, client_address_size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_realname;
		std::string _servername;

		bool		_isRegistered;
		bool		_isAuthenticated;

	public:

		Client(int clientFd);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);

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