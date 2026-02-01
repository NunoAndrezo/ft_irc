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
#include <netdb.h>

#define BUFFER_SIZE 1024


int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int portnumber = 54000;
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
    server.sin_addr.s_addr = inet_addr("0.0.0.0");
    //permite reutilizar a mesma socket imediatamente
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr << "Setsockopt failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }
/*     //set socket to non-blocking
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "Fcntl failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    } */
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
    //accept a call
    char host[NI_MAXHOST]; // client's remote name
    char service[NI_MAXSERV]; // service (i.e. port) the client is connect on
    // limpar memoria para nao ocupar com garbage values
    memset(&client, 0, sizeof(client));
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    int client_fd = accept(server_fd, (sockaddr*)&client, &clientSize);
    if (client_fd == -1)
    {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        close(server_fd);
        close(client_fd);
        return 1;
    }
    if (getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        bytesReceived = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == -1)
        {
            std::cerr << "Recv failed: " << strerror(errno) << std::endl;
            break;
        }
        if (bytesReceived == 0)
        {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        std::cout << "Received: " << std::string(buffer, 0, bytesReceived) << std::endl;
        send(client_fd, buffer, bytesReceived + 1, 0); // echo it back
    }
    close(client_fd);
    close(server_fd);
    return 0;
}

