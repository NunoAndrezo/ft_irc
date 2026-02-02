/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 01:57:59 by nuno              #+#    #+#             */
/*   Updated: 2026/02/02 01:57:59 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <cstdio>
#include "../inc/Client.hpp"
#include "../inc/IRCmacros.hpp"

void processCommand(Client& client, std::string line, const std::string& serverPass, std::map<int, Client>& clients);
void sendReply(int fd, std::string code, std::string nick, std::string msg);
void sendNotice(int fd, std::string nick, std::string msg);


// need to add signal handling for graceful shutdown
//SIGINT to set a global bool stop = false; to allow your while(true) loop to exit and close all FDs gracefullys

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int port = std::atoi(argv[1]);
	std::string password = argv[2];
	// need to add checks for port and password validity

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0 || listen(server_fd, 10) < 0) {
		perror("Setup failed");
		return 1;
	}

	std::vector<pollfd> fds; // Vector of pollfd structures
	std::map<int, Client> clients; // Map of client FDs to Client objects
	pollfd p; p.fd = server_fd; p.events = POLLIN; p.revents = 0; // Server socket
	fds.push_back(p); // Add server socket to poll list

	std::cout << "--- IRC Server Started ---" << std::endl;

	while (true) {
		if (poll(&fds[0], fds.size(), -1) < 0) break;
		for (size_t i = 0; i < fds.size(); ++i) {
			if (fds[i].revents & POLLIN) {
				if (fds[i].fd == server_fd) {
					int c_fd = accept(server_fd, NULL, NULL);
					if (c_fd != -1) {
						fcntl(c_fd, F_SETFL, O_NONBLOCK);
						pollfd cp; cp.fd = c_fd; cp.events = POLLIN; cp.revents = 0;
						fds.push_back(cp);
						
						std::cout << "[LOG] New connection request on FD: " << c_fd << std::endl;
						clients.erase(c_fd); 
						clients.insert(std::make_pair(c_fd, Client(c_fd)));

						std::string m = ":ircserv NOTICE * :*** CONNECTED. Please register: ***\r\n";
						m += ":ircserv NOTICE * :1. PASS <password>\r\n";
						m += ":ircserv NOTICE * :2. NICK <nickname>\r\n";
						m += ":ircserv NOTICE * :3. USER <username> 0 * :<realname>\r\n";
						send(c_fd, m.c_str(), m.length(), 0);
					}
				} else {
					char buf[1024];
					int bytes = recv(fds[i].fd, buf, 1023, 0);
					
					Client& c = clients[fds[i].fd];
					
					if (bytes <= 0) {
						std::string name = c.getNickname().empty() ? "Unregistered Client" : c.getNickname();
						std::cout << "[LOG] " << name << " (FD " << fds[i].fd << ") has disconnected/timed out." << std::endl;
						close(fds[i].fd);
						clients.erase(fds[i].fd);
						fds.erase(fds.begin() + i--);
					} else {
						buf[bytes] = '\0';
						c.appendBuffer(buf);
						size_t pos;
						while ((pos = c.getBuffer().find_first_of("\r\n")) != std::string::npos) {
							std::string line = c.getBuffer().substr(0, pos);
							size_t next = c.getBuffer().find_first_not_of("\r\n", pos);
							if (next == std::string::npos) c.getBuffer().clear();
							else c.getBuffer().erase(0, next);
							
							if (!line.empty()) {
								if (line.find("QUIT") == 0) {
									std::string name = c.getNickname().empty() ? "Unregistered Client" : c.getNickname();
									std::cout << "[LOG] " << name << " (FD " << fds[i].fd << ") has left the server (QUIT)." << std::endl;
									close(fds[i].fd);
									clients.erase(fds[i].fd);
									fds.erase(fds.begin() + i--);
									break; 
								}
								processCommand(c, line, password, clients);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

void sendReply(int fd, std::string code, std::string nick, std::string msg) {
	std::string clientNick = nick.empty() ? "*" : nick;
	std::string reply = ":ircserv " + code + " " + clientNick + " " + msg + "\r\n";
	send(fd, reply.c_str(), reply.length(), 0);
}

void sendNotice(int fd, std::string nick, std::string msg) {
	std::string clientNick = nick.empty() ? "*" : nick;
	std::string reply = ":ircserv NOTICE " + clientNick + " :" + msg + "\r\n";
	send(fd, reply.c_str(), reply.length(), 0);
}

void processCommand(Client& client, std::string line, const std::string& serverPass, std::map<int, Client>& clients) {
	std::stringstream ss(line);
	std::string command;
	ss >> command;
	for (size_t i = 0; i < command.length(); i++) command[i] = toupper(command[i]);

	if (command.empty()) return;

	// 1. QUIT handled in main loop to ensure cleanup

	// 2. CAP
	if (command == "CAP") {
		send(client.getFd(), "CAP * LS :\r\n", 12, 0);
		return;
	}

	// 3. PASS
	if (command == "PASS") {
		if (client.isRegistered()) {
			sendReply(client.getFd(), ERR_ALREADYREGISTRED, client.getNickname(), ":You may not reregister");
			return;
		}
		std::string pass;
		ss >> pass;
		if (pass == serverPass) {
			client.setHasPass(true);
			sendNotice(client.getFd(), client.getNickname(), "Step 1/3: Password accepted. Now send NICK <nickname>.");
		} else {
			sendReply(client.getFd(), ERR_PASSWDMISMATCH, client.getNickname(), ":Password incorrect");
		}
		return;
	}

	// 4. THE BOUNCER
	if (!client.hasPass()) {
		sendReply(client.getFd(), ERR_NOTREGISTERED, "*", ":You must send PASS first");
		return; 
	}

	// 5. NICK
	if (command == "NICK") {
		std::string nick;
		if (!(ss >> nick)) {
			sendReply(client.getFd(), ERR_NONICKNAMEGIVEN, client.getNickname(), ":No nickname given");
			return;
		}
		for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
			if (it->second.getNickname() == nick && it->first != client.getFd()) {
				sendReply(client.getFd(), ERR_NICKNAMEINUSE, "*", nick + " :Nickname is already in use");
				return;
			}
		}
		client.setNickname(nick);
		sendNotice(client.getFd(), client.getNickname(), "Step 2/3: Nickname saved. Now send USER <username> 0 * :<realname>.");
	} 
	// 6. USER
	else if (command == "USER") {
		if (client.isRegistered()) {
			sendReply(client.getFd(), ERR_ALREADYREGISTRED, client.getNickname(), ":You may not reregister");
			return;
		}
		std::string user, host, server, real;
		if (!(ss >> user >> host >> server)) {
			sendReply(client.getFd(), ERR_NEEDMOREPARAMS, client.getNickname(), "USER :Not enough parameters");
		} else {
			size_t pos = line.find(":");
			real = (pos != std::string::npos) ? line.substr(pos + 1) : user;
			client.setUsername(user);
			client.setRealname(real);
			sendNotice(client.getFd(), client.getNickname(), "Step 3/3: User info saved.");
		}
	}
	// 7. PRIVMSG
	else if (command == "PRIVMSG") {
		if (!client.isRegistered()) {
			sendReply(client.getFd(), ERR_NOTREGISTERED, "*", ":You have not registered");
			return;
		}
		std::string target;
		ss >> target;
		size_t pos = line.find(":");
		if (pos != std::string::npos) {
			std::string msg = line.substr(pos);
			bool found = false;
			for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
				if (it->second.getNickname() == target) {
					std::string fullMsg = ":" + client.getNickname() + " PRIVMSG " + target + " " + msg + "\r\n";
					send(it->second.getFd(), fullMsg.c_str(), fullMsg.length(), 0);
					found = true;
					break;
				}
			}
			if (!found) sendReply(client.getFd(), ERR_NOSUCHNICK, client.getNickname(), target + " :No such nick");
		}
	}
	else {
		sendReply(client.getFd(), ERR_UNKNOWNCOMMAND, client.getNickname(), command + " :Unknown command");
	}

	// FINAL REGISTRATION CHECK
	if (!client.isRegistered() && client.hasPass() && !client.getNickname().empty() && !client.getUsername().empty()) {
		client.setIsRegistered(true);
		sendReply(client.getFd(), RPL_WELCOME, client.getNickname(), ":Welcome to the Internet Relay Network " + client.getNickname());
		std::cout << "[LOG] Client '" << client.getNickname() << "' (FD " << client.getFd() << ") successfully registered." << std::endl;
	}
}
