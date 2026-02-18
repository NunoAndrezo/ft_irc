/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:31:37 by nneves-a          #+#    #+#             */
/*   Updated: 2026/02/18 11:03:45 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include <signal.h>

bool g_serverRunning = true;

static void handle_signals();
static void signalHandler(int signum);

int main(int argc, char **argv)
{
	handle_signals();	
	if (argc == 3 || (argc == 4 && std::string(argv[3]) == "--debug"))
	{
		// Parse port
		std::string portStr = argv[1];
		if (portStr.find_first_not_of("0123456789") != std::string::npos)
		{
			std::cerr << "Error: Port is not valid (please input a numeric value)." << std::endl;
			return 1;
		}
		int port = std::atoi(argv[1]);
		std::string password = argv[2];

		Server ircServer(port, password, argc == 4 ? true : false);
		try
		{
			ircServer.serverRun();
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error: " << e.what() << std::endl; 
			return 1;
		}
	}
	else
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password> [--debug]" << std::endl;
		return 1;
	}
	return 0;
}

void handle_signals()
{
	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, SIG_IGN);
	std::signal(SIGQUIT, SIG_IGN);
	std::signal(SIGTSTP, SIG_IGN);
	std::signal(SIGPIPE, SIG_IGN);
	std::signal(SIGSEGV, SIG_IGN);
}

void signalHandler(int signum)
{
	(void)signum;
	std::cout << "\n[SIGNAL] Shutdown signal received. Cleaning up..." << std::endl;
	g_serverRunning = false;
}