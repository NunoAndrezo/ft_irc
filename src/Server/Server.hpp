/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:35:33 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/11 23:59:42 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <map>
#include <sys/socket.h>
#include <poll.h>
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

#define ERR_USERSDONTMATCH "502"

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
		
		bool hasClient(std::string nickname);
		Client* getClientByNickname(std::string nickname);
		
		// commands
		
		void processCommand(Client& client, std::string line);

		void cmdCap(Client& client);
		void cmdPass(Client& client, std::stringstream& ss);
		void cmdNick(Client& client, std::stringstream& ss);
		void cmdUser(Client& client, std::stringstream& ss);
		void cmdPrivmsg(Client& client, std::string line, std::stringstream& ss);
		void cmdMode(Client& client, std::stringstream& ss);
		void cmdJoin(Client &client, std::stringstream &ss);
		void cmdInvite(Client& client, std::stringstream& ss);
		void cmdKick(Client& client, std::stringstream& ss);

		// Fx

		void addPollfd(std::vector<pollfd>& fds, int fd, short events);

		Channel* getChannelByName(std::string name); // return null if not found

		void cleanupClient(int i);
		void handleDisconnect(int i, Client &c);
		


};


#endif