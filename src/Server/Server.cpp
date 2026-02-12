/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 23:58:13 by nuno              #+#    #+#             */
/*   Updated: 2026/02/12 00:29:26 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string password, bool debug):  
	_serverPort(port),
	_serverPassword(password),
	_debug(debug),
	_serverHostname("TestingServer")
{}

Server::~Server() {
	// Cleanup any remaining clients
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear();
	close(_serverSocketFd);
}

// --- CLEANUP HELPERS ---

void Server::cleanupClient(int i) {
	int fd = _pollfds[i].fd;
	
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end()) {
		if (_debug)
			std::cout << FRED("[LOG] Deleting Client object for FD ") << fd << std::endl;
		delete it->second; // Free memory (pointer)
		_clients.erase(it); // Remove from map
	}

	close(fd);
	_pollfds.erase(_pollfds.begin() + i); // Remove from poll vector
}

void Server::handleDisconnect(int i, Client &c) {
	std::string name = c.getNickname().empty() ? "Unregistered Client" : c.getNickname();
	std::cout << FRED("[LOG] ") << name << " (FD " << _pollfds[i].fd << ") has disconnected." << std::endl;
	cleanupClient(i);
}

// --- MESSAGE HANDLING ---

void Server::clientMessage(int i, Client &c) {
	char buf[BUFSIZ];
	// Read data, leave space for null terminator
	int bytes = recv(_pollfds[i].fd, buf, sizeof(buf) - 1, 0);

	if (bytes <= 0) {
		handleDisconnect(i, c); 
		return;
	}

	buf[bytes] = '\0';
	c.appendBuffer(buf); 

	size_t pos;
	// Industry standard: Only process when a full line (\n) is found
	while ((pos = c.getBuffer().find("\n")) != std::string::npos) {
		std::string line = c.getBuffer().substr(0, pos);
		
		// Trim \r if present
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		// Advance buffer
		c.setBuffer(c.getBuffer().substr(pos + 1));

		if (line.empty()) continue;

		if (_debug)
			std::cout << FYEL("[LOG] Processing: ") << line << std::endl;

		processCommand(c, line);

		// If the command (like QUIT) triggered a disconnection
		if (c.getWasDisconnected()) {
			cleanupClient(i);
			return; // Safety: client reference 'c' is now invalid
		}
	}
}

// --- SOCKET & CONNECTION LOGIC ---

void Server::newClientConnection() {
	struct sockaddr_in clientAddr;
	socklen_t clientSize = sizeof(clientAddr);
	int client_fd = accept(_serverSocketFd, (sockaddr*)&clientAddr, &clientSize);
	
	if (client_fd == -1)
		return;

	// Set non-blocking for safety
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	
	addPollfd(_pollfds, client_fd, POLLIN | POLLHUP);

	char hostname[NI_MAXHOST];
	getnameinfo((struct sockaddr *)&clientAddr, clientSize, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

	// Create new Client on the heap
	Client *clientObj = new Client(client_fd, _serverHostname, hostname, _debug);
	_clients.insert(std::make_pair(client_fd, clientObj));

	if (_debug)
		std::cout << "[LOG] Client " << hostname << " connected on FD " << client_fd << std::endl;
}

void Server::serverSocketStart() {
	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd == -1)
		throw std::runtime_error("Socket creation failed");

	int opt = 1;
	setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK);

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_serverPort);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_serverSocketFd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("Bind failed");

	if (listen(_serverSocketFd, SOMAXCONN) == -1)
		throw std::runtime_error("Listen failed");

	std::cout << "Server listening on port " << _serverPort << std::endl;
}

void Server::serverRun() {
	serverSocketStart();
	addPollfd(_pollfds, _serverSocketFd, POLLIN);
	addPollfd(_pollfds, STDIN_FILENO, POLLIN);

	while (true) {
		if (poll(_pollfds.data(), _pollfds.size(), -1) == -1)
			throw std::runtime_error("Poll failed");

		// 1. New Connections
		if (_pollfds[0].revents & POLLIN)
			newClientConnection();

		// 2. Server Console (STDIN)
		if (_pollfds[1].revents & POLLIN) {
			std::string cmd;
			std::getline(std::cin, cmd);
			if (cmd == "EXIT") break;
		}

		// 3. Client Messages (Looping carefully due to possible erasures)
		for (size_t i = 2; i < _pollfds.size(); ) {
			bool increment = true;
			int fd = _pollfds[i].fd;

			if (_pollfds[i].revents & POLLIN) {
				clientMessage(i, *_clients[fd]);
				// Check if clientMessage resulted in a cleanup (vector size changed)
				if (i >= _pollfds.size() || _pollfds[i].fd != fd)
					increment = false;
			} 
			else if (_pollfds[i].revents & (POLLHUP | POLLERR)) {
				handleDisconnect(i, *_clients[fd]);
				increment = false;
			}

			if (increment) i++;
		}
	}
}

// --- UTILS ---

void Server::addPollfd(std::vector<pollfd>& fds, int fd, short events) {
	pollfd p;
	p.fd = fd;
	p.events = events;
	p.revents = 0;
	fds.push_back(p);
}

Channel *Server::getChannelByName(std::string name) {
	for (size_t i = 0; i < _channels.size(); ++i) {
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return NULL;
}

Client *Server::getClientByNickname(std::string nickname) {
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second->getNickname() == nickname)
			return it->second;
	}
	return NULL;
}

bool Server::hasClient(std::string nickname)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
			return true;
	}
	return false;
}