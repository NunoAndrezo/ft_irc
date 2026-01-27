#include "ServerObj.hpp"




ServerObj::ServerObj(){

// vazio nn serve


}

ServerObj::~ServerObj(){
	//close all sockets
		// Clean up listening and remaining client sockets before shutting down
	//cleanup resources
		// Free memory, remove state objects to prevent leaks
}

ServerObj::ServerObj(com os argumentos do prompt){

	//parse command-line arguments (port, password)
	parsing()
		// We need the server port and connection password because the server cannot start without knowing where to listen and what password to enforce
	//validate arguments
		// Ensures user input is correct so the server doesn’t crash or misbehave later. Check for valid port range and non-empty password So people like Hugo don't fuck with out project
	//initialize server state
		// Create data structures for clients, channels, and global configuration, because the server needs to track everything consistently

}

// fazer copy

// fazer assignment operator

// funcoes de parsing