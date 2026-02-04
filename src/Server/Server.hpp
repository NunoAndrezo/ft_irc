/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:35:33 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/04 15:44:16 by toferrei         ###   ########.fr       */
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
#include "../Channel/Channel.hpp"
#include "../utils/IRCmacros.hpp"
#include "../utils/stdoutColors.hpp"

class Channel;

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
		std::vector<Channel*>	_channels;

	public:
		Server(int port, std::string password, bool debug);
		~Server();
		Server(const Server &src);
		Server &operator=(const Server &src);

		void serverRun();
		void serverSocketStart();
		void newClientConnection();
		void clientMessage(int i, Client &c);
		void processCommand(Client& client, std::string line);

		bool hasClient(std::string nickname);
		Client* getClientByNickname(std::string nickname);

};

#endif