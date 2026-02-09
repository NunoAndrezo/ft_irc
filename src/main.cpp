/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:31:37 by nneves-a          #+#    #+#             */
/*   Updated: 2026/02/09 18:24:49 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include <signal.h>



int main(int argc, char **argv)
{
	// signal(SIGINT, SIG_IGN); // ignore SIGINT to let the server handle it gracefully
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