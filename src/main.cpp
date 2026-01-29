/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 18:35:18 by nuno              #+#    #+#             */
/*   Updated: 2026/01/29 17:30:01 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int	main()
{
	
	// Create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return -1;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];      // Client's remote name
	char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

	memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	close(listening);

	// While loop: accept and echo message back to client
	char buf[4096];

	while (true)
	{
		memset(buf, 0, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == -1)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		cout << string(buf, 0, bytesReceived) << endl;

		// Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	// Close the socket
	close(clientSocket);



	//START PROGRAM

	//parse command-line arguments (port, password)
		// We need the server port and connection password because the server cannot start without knowing where to listen and what password to enforce
	//validate arguments
		// Ensures user input is correct so the server doesn’t crash or misbehave later. Check for valid port range and non-empty password So people like Hugo don't fuck with out project
	//initialize server state
		// Create data structures for clients, channels, and global configuration, because the server needs to track everything consistently
	//create listening socket // Required to accept incoming client connections; without it, the server cannot receive anyone
	//configure socket options // Set options like SO_REUSEADDR to prevent restart issues and improve stability
	//set socket to non-blocking // Non-blocking is required to avoid freezing the server while waiting for network I/O
	//bind socket to IP and port // Assign the socket to the specific port/IP so the OS knows where to deliver incoming data
	//listen on socket // Marks the socket as ready to accept connections; enables connection queueing
	//initialize poll descriptor list // We need to track all file descriptors for event-driven I/O
	//add listening socket to poll list // The listening socket is part of poll so we can detect incoming connection requests


	//@1stWHILE server is running
	//poll all file descriptors // Wait for any socket (server or client) to have activity without blocking

	//	%FOR each descriptor with an event
	
	//		IF event is on listening socket
	//			accept new client // Create a new connection for each incoming client
	//			set client socket to non-blocking // Ensure individual clients don’t block the server
	//			create client state // Track client authentication, buffers, nickname, channels, etc., needed for IRC rules
	//			add client socket to poll list // So we can receive future messages from this client
	//		ELSE IF event is on a client socket
	//			receive data from client // Read incoming bytes from the client
	//			IF client disconnected or error
	//				remove client from all channels // Maintain consistency in channel state
	//				remove client from poll list // Stop monitoring the closed socket
	//				close client socket // Release system resources
	//			ELSE
	//				append data to client buffer // Buffer incomplete data because TCP streams may be fragmented
	//				WHILE buffer contains complete command
	//					extract one command // Split the buffered data into a single IRC command
	//					parse command // Identify command type and parameters
	//					validate client state // Ensure client is authenticated and allowed to execute the command
	//					execute command // Update channels, send responses, enforce rules
	//					send responses as needed // Reply to client(s) or broadcast messages as required
	//	END IF
	//%END FOR
	//@END 1stWHILE

	//close all sockets // Clean up listening and remaining client sockets before shutting down
	//cleanup resources // Free memory, remove state objects to prevent leaks
	//END PROGRAM

	return (0);
}