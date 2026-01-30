/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 18:35:18 by nuno              #+#    #+#             */
/*   Updated: 2026/01/30 10:21:24 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

int	main()
{
	// To test in another terminal: just use: nc 127.0.0.1(because this IP always routes to local host, our server is listning on it because of 0.0.0.0) <port we are using in this case: 51000>
	// we can also use the machine's LAN IP address to connect from another device in the same network.
	// we can find it with: ifconfig (linux/mac) or ipconfig (windows)
	// tldr - in this case: terminal 1 runs the ./out
	//terminal 2: nc 0.0.0.0 51000
	int	listening;
	int	port_num;
	
	// Create the socket
	listening = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4, SOCK_STREAM = TCP
	port_num = 51000; // Port number to listen on, in this case we selected 51000 because it's above 50000 (the range for dynamic/private ports) to avoid conflicts with well-known services.
	//51000 a random choice though, any port above 1024 would work for testing. Preferably above 50000 to avoid conflicts with other services.
	if (listening == -1)
		return (cerr << "Can't create a socket! Quitting" << endl, 1);

	// Creating IP adrress and port and then Binding the ip address and port -> to the socket.
	// setting up the sockaddr_in structure with the IP address and port, and then binding it to the socket.
	sockaddr_in serverAddress; // struct that specifies the address family, IP address, and port for the socket.
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port_num); // Port number. We are using htons to convert from host byte order to network byte order (little-endian to big-endian) - why? Because different systems may use different byte orders, and network protocols typically use big-endian order.
	inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr); // Here we are telling the socket to bind to all available interfaces on the machine.
	// Bind the socket to the IP address and port - here
	if (bind(listening, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		return (cerr << "Can't bind to IP/port! Quitting" << endl, 2);

	// Tell Winsock the socket is for listening
	if (listen(listening, SOMAXCONN) == -1) // Here we are telling the socket to listen for incoming connections. SOMAXCONN is a constant that specifies the maximum length for the queue of pending connections.
		return (cerr << "Can't listen! Quitting" << endl, 3);
	cout << "Server listening -> Port: " << port_num << endl;
	// Wait for a connection
	sockaddr_in client; // Client address. Contains information about the client that is connecting to the server.
	socklen_t clientSize = sizeof(client); // Size of the client address structure. For accept function.

	int clientSocket = accept(listening, (sockaddr*)&client, &clientSize); // Accept a call/connection from a client
	if (clientSocket == -1)
		return (cerr << "Problem with client connecting! Quitting" << endl, 4);
	// Display client information
	char host[NI_MAXHOST];	// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	memset(host, 0, NI_MAXHOST); // Initialize memory to zero before using it because getnameinfo may not write to all bytes
	memset(service, 0, NI_MAXSERV); // same as above

	// Try to get the host name and service (port). If it fails, we use inet_ntop to get the IP address instead.
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
	close(listening); // we should not close the listening socket here in a real server, because we want to keep accepting new connections.
	// REALLY IMPORTANTE! In this simple example, we close it because we are only handling one client connection.
	// In a real server we need to swap to: poll() or select() to handle multiple clients simultaneously.


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
		send(clientSocket, buf, bytesReceived, 0); // send() function to send data back to the client
	}

	// Close the socket
	close(clientSocket);

	//with this little program we are not able to connect an irc client to the server and talk with it.
	//Because we need to implement the IRC protocol correctly in the code.
	//This code is a basic TCP echo server, which means it can accept connections and echo back any messages it receives. However, to function as a proper IRC server,
	//We are still missing the basic commands and behaviors defined by the IRC protocol. Such as NICK, USER, JOIN, PRIVMSG, etc.
	//We would need to parse these commands from the client's messages and respond appropriately according to the IRC protocol specifications.
	//So all we're missing is parsing and adding commands according to the IRC protocol.
	//We need to parse the commands sent by the clients and handle them according to the IRC protocol.
	//This involves recognizing commands like NICK, USER, JOIN, PRIVMSG, and others, extracting their parameters, and executing the appropriate actions (e.g., setting nicknames, joining channels, sending messages).
	//Additionally, we'll need to manage the state of connected clients, channels, and enforce IRC rules.
	//Implementing these features will allow our server to interact properly with IRC clients.
	// we are also missing authentication and user management! We need to manage user states, including authentication (NICK/USER commands), tracking which users are in which channels, and enforcing permissions and modes.
	// also handling edge cases and errors according to the IRC protocol would be necessary for a robust implementation.
	// and finally, we need to ensure that our server can handle multiple clients simultaneously, which may involve using non-blocking sockets or multi-threading.


/*
Why IRC clients won’t “talk” to it

IRC requires:

\r\n terminated commands

Mandatory handshake:

PASS

NICK

USER

Server numeric replies (001, 433, etc.)

Stateful clients

Message routing

Your server:

Echoes raw bytes

Doesn’t parse commands

Doesn’t send valid IRC replies

So an IRC client will connect, then immediately disconnect.
*/

	return (0);












	// PSEUDOCODE:
	
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

}