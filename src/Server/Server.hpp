/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:35:33 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/03 00:32:03 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <netdb.h>
#include <sstream>
#include <cstdio>

#include "../Client/Client.hpp"
#include "../utils/IRCmacros.hpp"

class Client;

class Server
{
	private:
		const int				_serverPort;
		const std::string		_serverPassword;
		bool					_debug;
		const std::string		_serverHostname;

		int 					_serverSocketFd;

		std::vector<pollfd>		_pollfds;
		std::map<int, Client*>	_clients;

	public:
		Server(int port, std::string password, bool debug);
		~Server();
		Server(const Server &src);
		Server &operator=(const Server &src);

		void serverRun();
		void serverSocketStart();
		void newClientConnection();

};

#endif