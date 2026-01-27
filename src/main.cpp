/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 18:35:18 by nuno              #+#    #+#             */
/*   Updated: 2026/01/27 19:03:50 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


int	main(void)
{
	try{
		ServerObj server;
	}
	catch{

	}






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