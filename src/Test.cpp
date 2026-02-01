/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 15:15:53 by marvin            #+#    #+#             */
/*   Updated: 2026/01/28 15:15:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <vector>

#define BUFFER_SIZE 1024

void addPollfd(std::vector<pollfd>& fds, int fd, short events) // copied from https://github.com/caroldaniel/42sp-cursus-ft_irc/
{
	pollfd p;
    p.fd = fd;
    p.events = events;
    p.revents = 0;
    fds.push_back(p);
}

int main()
{
	std::vector<pollfd> pollfds;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int portnumber = 51212;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    //create socket
    if (server_fd == -1)
    {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        return 1;
    }
    std::cout << "Socket created successfully." << std::endl;
    memset(&server, 0, sizeof(server)); // limpar memoria para nao ocupar com garbage values
    //bind the socket to ip/port
    server.sin_family = AF_INET;
    server.sin_port = htons(portnumber);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    //permite reutilizar o endereço imediatamente
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr << "Setsockopt failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }
    //set socket to non-blocking
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Fcntl failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }
    // bind the socket to the IP and port
    if (bind(server_fd, (sockaddr*)&server, sizeof(server)) == -1)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }
    //mark the socket for listening
    if (listen(server_fd, SOMAXCONN) == -1) // SOMAXCONN is the max number of connections allowed in the incomming queue
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }
    std::cout << "Server listening on port " << portnumber << std::endl;
    //accept a call TODO APARTIR DAQUI
    memset(&client, 0, sizeof(client));



    // int client_fd = accept(server_fd, (sockaddr*)&client, &clientSize);
/*     if (client_fd == -1)
    {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }
    std::cout << "Client connected!" << std::endl; */
    addPollfd(pollfds, server_fd, POLLIN);
    addPollfd(pollfds, STDIN_FILENO, POLLIN);    
    while (1)
	{
		int poll_count = poll(pollfds.data(), pollfds.size(), 0);
		if (poll_count == - 1)
			return 1;
		if (poll_count == 0)
			continue;
		if (pollfds[0].revents & POLLIN)
		{
			int client_fd = accept(server_fd, (sockaddr*)&client, &clientSize);
			if (client_fd == -1)
			{
				std::cerr << "Accept failed: " << strerror(errno) << std::endl;
			}
		    std::cout << "Client connected!" << std::endl;
			addPollfd(pollfds, client_fd, POLLIN | POLLHUP);
			send(client_fd, ":server 001 etom :Welcome to the server!\n", 42, 0); // handshake message :
			// \_____> criacao clientes




		}
		if (pollfds[1].revents & POLLIN) {
            char buffer[1024];
            int bytes_received = read(STDIN_FILENO, buffer, sizeof(buffer));
			std::cout << buffer << std::endl;		
			break ;
            // Check for errors in the read function
        }
		char buffer[BUFFER_SIZE];
		for (size_t i = 2; i < pollfds.size(); ++i)
		{
			if ((pollfds[i].revents & POLLHUP) == POLLHUP)
			{
				std::cout << "Client disconnected!" << std::endl; // When client disconnects shows message
				close (pollfds[i].fd);
				pollfds.erase(pollfds.begin() + i);
			}
			if ((pollfds[i].revents & POLLIN) == POLLIN)
			{
				// aqui faz-se parse dos commandos
                memset(buffer, '\0', BUFFER_SIZE);
				if (recv(pollfds[i].fd, buffer, sizeof(buffer), 0) > 0)
				{
					std::cout << buffer << std::endl;
                    if (strncmp(buffer, "PING", 4) == 0) //example of command TODO
                    {
                        std::string response = "PONG" + std::string(buffer + 4);
                        send(pollfds[i].fd, response.c_str(), response.length(), 0);    
					}
				}

			}
		}
    }
    close(server_fd);
    return 0;
}

