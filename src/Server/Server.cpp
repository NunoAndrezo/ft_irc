/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:36:09 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/09 18:04:51 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, std::string password, bool debug):  _serverPort(port),
															_serverPassword(password),
															_debug(debug),
															_serverHostname("TestingServer")
{

}

Server::~Server()
{
	close(_serverSocketFd);
}

Server::Server(const Server &src): _serverPort(src._serverPort),
							_serverPassword(src._serverPassword),
							_debug(src._debug),
							_serverHostname(src._serverHostname),
							_serverSocketFd(src._serverSocketFd),
							_pollfds(src._pollfds),
							_clients(src._clients),
							_channels(src._channels)
{

}

Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		// _serverPort, _serverPassword, _serverHostname are const, cannot be assigned
		_debug = src._debug;
		_serverSocketFd = src._serverSocketFd;
		_pollfds = src._pollfds;
		_clients = src._clients;
		_channels = src._channels;
	}
	return *this;
}

void Server::clientMessage(int i, Client &c)
{
	char buf[BUFSIZ];
	bzero(buf, sizeof(buf));
	int bytes = recv(_pollfds[i].fd, buf, sizeof(buf), 0);
	if (_debug)
		std::cout << FRED("[LOG] Received ") << bytes << " bytes" << std::endl;
	if (bytes <= 0)
	{
		if (bytes == 0)
		{
			std::string name = c.getNickname().empty() ? "Unregistered Client" : c.getNickname();
			std::cout << FRED("[LOG] ") << name << " (FD " << _pollfds[i].fd << ") has disconnected." << std::endl;
		}
		else
			std::cerr << "Recv failed: " << std::string(strerror(errno)) << std::endl;
		c.setWasDisconnected(true);
		close(_pollfds[i].fd);
		_clients.erase(_pollfds[i].fd);
		_pollfds.erase(_pollfds.begin() + i);
		return;
	}

	c.appendBuffer(buf);
	
	size_t pos;
	while (!c.getBuffer().empty())
	{
		if (c.getBuffer().find(CRLF) == std::string::npos)
			return; // wait for more data
		pos = c.getBuffer().find(CRLF);
		std::string line = c.getBuffer().substr(0, pos);
		c.setBuffer(c.getBuffer().substr(pos + 2)); // remove processed line + CRLF

		if (line.find("QUIT") == 0) // deviamos tratar o quit como um commando normal
		{
			std::string name = c.getNickname().empty() ? "Unregistered Client" : c.getNickname();
			std::cout << "[LOG] " << name << " (FD " << _pollfds[i].fd << ") has left the server (QUIT)." << std::endl;
			c.reply(RPL_QUIT, ":Client quitting");
			c.setWasDisconnected(true);
			close(_pollfds[i].fd);
			_clients.erase(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
			delete &c;
			return; 
		}
		if (_debug)
			std::cout << FYEL("[LOG] Processing command from FD ") << _pollfds[i].fd << ": " << line << std::endl;
		processCommand(c, line);
				
		// Check if client was disconnected after/during processing commands
		if (c.getWasDisconnected())
		{
			std::string name = c.getNickname().empty() ? "Unregistered Client" : c.getNickname();
			std::cout << "[LOG] " << name << " (FD " << _pollfds[i].fd << ") disconnected." << std::endl;
			close(_pollfds[i].fd);
			_clients.erase(_pollfds[i].fd);
			_pollfds.erase(_pollfds.begin() + i);
			return;
		}
	}
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
	int result = getnameinfo((struct sockaddr *)&client, clientSize, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
	if (result != 0)
		throw std::runtime_error(std::string(gai_strerror(result)));
	Client *clientObj = new Client(client_fd, _serverHostname, hostname, _debug);
	_clients.insert(std::make_pair(client_fd, clientObj));
	std::cout << "Client: " << clientObj->getHostname() << ":" << clientObj->getFd() << " has connected" << std::endl;	// nn sei se faz sentido mandar isto aqui ja que se manda a pass ao mesmo tempo que se liga
}

void Server::serverSocketStart()
{
	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd == -1)
	{
		throw std::runtime_error("Socket creation failed " + std::string(strerror(errno)));
	}
	std::cout << "Socket created successfully." << std::endl;
	//make the socket reusable
	int opt = 1;
	if (setsockopt(_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(_serverSocketFd);
		throw std::runtime_error("Setsockopt failed: " + std::string(strerror(errno)));
	}
	//make socket non-blocking
	if (fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_serverSocketFd);
		throw std::runtime_error("Fcntl failed: " + std::string(strerror(errno)));
	}
	//define server address structure
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_serverPort);
	server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	//bind server socket to server address and port
	if (bind(_serverSocketFd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		close(_serverSocketFd);
		throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
	}
	//listen for incoming connections
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
		std::cerr << e.what() << std::endl;
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
		//if no events occurred, continue
		if (pollCount == 0)
			continue;
 		//debug purposes
		if (_debug)
		{
			std::cout << "[LOG] Poll returned " << pollCount << " events." << std::endl;
			for (size_t i = 0; i < _pollfds.size(); ++i)
			{
				if (_pollfds[i].revents != 0)
				{
					std::cout << "[LOG] FD " << _pollfds[i].fd << ":i:" << i << " has events: " << _pollfds[i].revents << std::endl;
					if (_pollfds[i].revents & POLLIN)
						std::cout << "[LOG]   - POLLIN event" << std::endl;
					if (_pollfds[i].revents & POLLHUP)
						std::cout << "[LOG]   - POLLHUP event" << std::endl;
					if (_pollfds[i].revents & POLLNVAL)
						std::cout << "[LOG]   - POLLNVAL event" << std::endl;
				}
			}
		}
		//check for new connections
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
				continue;
			}
		}
		//check for stdin input (server commands)
		if (_pollfds[1].revents & POLLIN) // read from stdin
		{	
			char buffer[BUFSIZ];
			int bytes_received = read(STDIN_FILENO, buffer, sizeof(buffer));
			if (bytes_received <= 0)
				throw std::runtime_error("Error reading from stdin");
			std::string bufstring(buffer);
			//exit server on "EXIT" command
			if (bufstring.find("EXIT") != std::string::npos)
				break ;
		}
		//check for client messages
		for (size_t i = 2; i < _pollfds.size(); ++i)
		{
/* 			if (_clients[_pollfds[i].fd]->getWasDisconnected())
			{
				continue;
			} */
			if (_debug)
				std::cout << "FD " << _pollfds[i].fd << " revents: " << _pollfds[i].revents << std::endl;
			if (_clients.find(_pollfds[i].fd) == _clients.end())
			{
				if (_debug)
					std::cout << "[LOG] FD " << _pollfds[i].fd << " not found in clients map, skipping." << std::endl;
				continue;
			}
			//check for incoming data client side
			if (_pollfds[i].revents & POLLIN)
			{
				if (_debug)
					std::cout << "[LOG] Data incoming on FD: " << _pollfds[i].fd << std::endl;
				Server::clientMessage(i, *_clients[_pollfds[i].fd]);
			}
		}
	}
}

bool Server::hasClient(std::string nickname)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second->getNickname() == nickname)
			return true;
	return false;
}

Client *Server::getClientByNickname(std::string nickname)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second->getNickname() == nickname)
			return it->second;
	return NULL;
}

void Server::addPollfd(std::vector<pollfd>& fds, int fd, short events)
{
	pollfd p;
	p.fd = fd;
	p.events = events;
	p.revents = 0;
	fds.push_back(p);
}

Channel *Server::getChannelByName(std::string name)
{
	for (size_t i = 0; i < _channels.size(); ++i)
	{
		if (_channels[i]->getName() == name)
			return _channels[i];
	}
	return NULL;
}