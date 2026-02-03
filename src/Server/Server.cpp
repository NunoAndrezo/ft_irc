/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:36:09 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/03 00:36:06 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//  nao sei aonde meter isso

void processCommand(Client& client, std::string line, const std::string& serverPass, std::map<int, Client*>& clients)
{
	std::stringstream ss(line);
	std::string command;
	ss >> command;
	for (size_t i = 0; i < command.length(); i++)
		command[i] = toupper(command[i]);

	if (command.empty())
		return;

	// 1. QUIT handled in main loop to ensure cleanup

	// 2. CAP
	if (command == "CAP")
	{
		// client.reply(RPL_CAP, ":No supported capabilities available");
		send(client.getFd(), "CAP * LS :\r\n", 12, 0);
		return;
	}

	// 3. PASS
	if (command == "PASS")
	{
		if (client.getIsRegistered())
		{
			client.reply(ERR_ALREADYREGISTRED, ":You may not reregister");
			return;
		}
		std::string pass;
		ss >> pass;
		if (pass == serverPass)
		{
			client.setHasPass(true);
			client.reply(RPL_NOTICE, "Step 1/3: Password accepted. Now send NICK <nickname>.");
		}
		else
		{
			client.reply(ERR_PASSWDMISMATCH, ":Password incorrect");
		}
		return;
	}

	// 4. THE BOUNCER
	if (!client.getHasPass())
	{
		client.reply(ERR_NOTREGISTERED, ":You must send PASS first");
		return; 
	}

	// 5. NICK
	if (command == "NICK") 
	{
		std::string nick;
		if (!(ss >> nick))
		{
			client.reply(ERR_NONICKNAMEGIVEN, ":No nickname given");
			return;
		}
		for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->second->getNickname() == nick && it->first != client.getFd())
			{
				client.reply(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use");
				return;
			}
		}
		client.setNickname(nick);
		client.reply(RPL_NOTICE, "Step 2/3: Nickname saved. Now send USER <username> 0 * :<realname>.");
	} 
	// 6. USER
	else if (command == "USER")
	{
		if (client.getIsRegistered())
		{
			client.reply(ERR_ALREADYREGISTRED, ":You may not reregister");
			return;
		}
		std::string user, host, server, real;
		if (!(ss >> user >> host >> server))
		{
			client.reply(ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
		}
		else
		{
			size_t pos = line.find(":");
			real = (pos != std::string::npos) ? line.substr(pos + 1) : user;
			client.setUsername(user);
			client.setRealname(real);
			client.reply(RPL_NOTICE, "Step 3/3: User info saved.");
		}
	}
	// 7. PRIVMSG
	else if (command == "PRIVMSG")
	{
		if (!client.getIsRegistered()) {
			client.reply(ERR_NOTREGISTERED, ":You have not registered");
			return;
		}
		std::string target;
		ss >> target;
		size_t pos = line.find(":");
		if (pos != std::string::npos)
		{
			std::string msg = line.substr(pos);
			bool found = false;
			for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
			{
				if (it->second->getNickname() == target)
				{
					std::string fullMsg = ":" + client.getNickname() + " PRIVMSG " + target + " " + msg + "\r\n";
					send(it->second->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
					found = true;
					break;
				}
			}
			if (!found)
				client.reply(ERR_NOSUCHNICK, target + " :No such nick");
		}
	}
	else
	{
		client.reply(ERR_UNKNOWNCOMMAND, command + " :Unknown command");
	}

	// FINAL REGISTRATION CHECK
	if (!client.getIsRegistered() && client.getHasPass() && !client.getNickname().empty() && !client.getUsername().empty()) {
		client.setIsRegistered(true);
		client.reply(RPL_WELCOME, ":Welcome to the Internet Relay Network " + client.getNickname());
		std::cout << "[LOG] Client '" << client.getNickname() << "' (FD " << client.getFd() << ") successfully registered." << std::endl;
	}
}








// server utils

void addPollfd(std::vector<pollfd>& fds, int fd, short events)
{
	pollfd p;
	p.fd = fd;
	p.events = events;
	p.revents = 0;
	fds.push_back(p);
}


Server::Server(int port, std::string password, bool debug):  _serverPort(port),
															_serverPassword(password),
															_debug(debug)
{

}

Server::~Server()
{
	close(_serverSocketFd);
}

void Server::newClientConnection()
{
	
	struct sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	int client_fd = accept(_serverSocketFd, (sockaddr*)&client, &clientSize);
	if (client_fd == -1)
		throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
	addPollfd(_pollfds, client_fd, POLLIN | POLLHUP);
	if (_debug)
		std::cout << "[LOG] New connection request on FD: " << client_fd<< std::endl;
	char hostname[NI_MAXHOST];
	int result = getnameinfo((struct sockaddr *)&client, clientSize, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
	if (result != 0)
		throw std::runtime_error(std::string(gai_strerror(result)));
	Client *clientObj = new Client(client_fd, _serverHostname);
	_clients.insert(std::make_pair(client_fd, clientObj));

	std::cout << "Client connected!" << std::endl;
	// nn sei se faz sentido mandar isto aqui ja que se manda a pass ao mesmo tempo que se liga
	// std::string m = ":ircserv NOTICE * :*** CONNECTED. Please register: ***\r\n";
	// m += ":ircserv NOTICE * :1. PASS <password>\r\n";
	// m += ":ircserv NOTICE * :2. NICK <nickname>\r\n";
	// m += ":ircserv NOTICE * :3. USER <username> 0 * :<realname>\r\n";
	clientObj->reply(RPL_WELCOME, "Connection Established");
	// send(client_fd, ":server 001 etom :Welcome to the server!\n", 42, 0); // handshake message :
	
}

void Server::serverSocketStart()
{
	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd == -1)
	{
		throw std::runtime_error("Socket creation failed " + std::string(strerror(errno)));
	}
	std::cout << "Socket created successfully." << std::endl;
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(_serverPort);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	int opt = 1;
	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(_serverSocketFd);
		throw std::runtime_error("Setsockopt failed: " + std::string(strerror(errno)));
	}
	if (fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_serverSocketFd);
		throw std::runtime_error("Fcntl failed: " + std::string(strerror(errno)));
	}
	if (bind(_serverSocketFd, (sockaddr*)&server, sizeof(server)) == -1)
	{
		close(_serverSocketFd);
		throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
	}
	if (listen(_serverSocketFd, SOMAXCONN) == -1) // SOMAXCONN is the max number of connections allowed in the incomming queue
	{
		close(_serverSocketFd);
		throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
	}
	std::cout << "Server listening on port " << _serverPort << std::endl;
}

void Server::serverRun()
{
	try
	{
		Server::serverSocketStart();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		throw std::runtime_error("Server startup failed: " + std::string(strerror(errno)));
	}
	addPollfd(_pollfds, _serverSocketFd, POLLIN);
	addPollfd(_pollfds, STDIN_FILENO, POLLIN);
	if (_debug)
		std::cout << "[LOG] Server is running. Waiting for connections..." << std::endl;
	while (true)
	{
		int pollCount = poll(_pollfds.data(), _pollfds.size(), 0);
		if (pollCount == - 1)
			throw std::runtime_error("Error: Poll failed" + std::string(strerror(errno)));
		if (pollCount == 0)
			continue;
		// std::cout << "[LOG] Poll returned " << pollCount << " events." << std::endl;
		if (_pollfds[0].revents & POLLIN)
		{
			try	
			{
				if (_debug)
					std::cout << "[LOG] New client connection incoming..." << std::endl;
				Server::newClientConnection();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		if (_pollfds[1].revents & POLLIN) // read from stdin
		{	
			char buffer[BUFSIZ];
			int bytes_received = read(STDIN_FILENO, buffer, sizeof(buffer));
			if (bytes_received <= 0)
				throw std::runtime_error("Error reading from stdin");
			std::string bufstring(buffer);	
			if (bufstring.find("EXIT") != std::string::npos)
				break ;
			// Check for errors in the read function
		}
		for (size_t i = 2; i < _pollfds.size(); ++i)
		{
			if (_pollfds[i].revents & POLLHUP)
			{
					std::string name = _clients[_pollfds[i].fd]->getNickname().empty() ? "Unregistered Client" : _clients[_pollfds[i].fd]->getNickname();
					std::cout << "[LOG] " << name << " (FD " << _pollfds[i].fd << ") has disconnected/timed out." << std::endl;
					close(_pollfds[i].fd);
					_clients.erase(_pollfds[i].fd); // tomaz -- acho que podemos deixar ate para a destruicao do server e assim faz-se delete tb
					_pollfds.erase(_pollfds.begin() + i--);
			}
			if (_pollfds[i].revents & POLLIN) // a forma como isso esta escrito faz com que ele volte a recomecar o loop se a mensagem recebida for maior que o buffer
			{
				char buf[BUFSIZ];
				// std::cout << "ola" << std::endl;
				int bytes = recv(_pollfds[i].fd, buf, sizeof(buf), 0);
				Client *c = _clients[_pollfds[i].fd];
				if (bytes < 0)
				{
					throw std::runtime_error("Recv failed: " + std::string(strerror(errno)));
				}
				else
				{
					buf[bytes] = '\0';
				c->appendBuffer(buf);
				size_t pos;
				while ((pos = c->getBuffer().find_first_of("\r\n")) != std::string::npos) // tomaz -- problema esta aqui nesse while penso eu
				{
					std::string line = c->getBuffer().substr(0, pos);
					size_t next = c->getBuffer().find_first_not_of("\r\n", pos);
					if (next == std::string::npos)
						c->getBuffer().clear();
					else
						c->getBuffer().erase(0, next);
					if (!line.empty())
					{
						if (line.find("QUIT") == 0) // deviamos tratar o quit como um commando normal
						{
							std::string name = c->getNickname().empty() ? "Unregistered Client" : c->getNickname();
							std::cout << "[LOG] " << name << " (FD " << _pollfds[i].fd << ") has left the server (QUIT)." << std::endl;
							close(_pollfds[i].fd);
							_clients.erase(_pollfds[i].fd);
							_pollfds.erase(_pollfds.begin() + i--);
							delete c;
							break; 
						}
						processCommand(*c, line, _serverPassword, _clients);
						std::cout << "remaining buffer :" << c->getBuffer() << "buffer ate aqui" << std::endl;
						c->clearBuffer(); // Clear buffer after processing (essa linha esta aqui porque senao o server entala num loop com o cliente por cause do CAP)
					}
					}
				}

			}
		}
	}
}
