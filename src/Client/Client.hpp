/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:42:42 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/01 17:53:25 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client { // int result = getnameinfo((struct sockaddr *)&client_address, client_address_size, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		bool		_isRegistered;

	public:

		Client(int fd);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);

		void clientRegistration();
		int clientAuthentication();

		int getFd() const;
		void setFd(const int fd);
		std::string getNickname() const;
		void setNickname(const std::string& nickname);
		std::string getUsername() const;
		void setUsername(const std::string& username);
		bool getIsRegistered() const;
		void setIsRegistered(const bool status);

};

#endif