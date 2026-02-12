/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:15:15 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/12 00:29:22 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::processCommand(Client& client, std::string line)
{
	std::stringstream ss(line);
	std::string command;
	ss >> command;
	for (size_t i = 0; i < command.length(); i++)
		command[i] = toupper(command[i]);

	if (command.empty()) return;

	// --- 1. THE "OPEN" COMMANDS ---
	// These MUST be allowed even if the user is unregistered
	if (command == "CAP")
		cmdCap(client);
	else if (command == "PASS")
		cmdPass(client, ss);
	else if (command == "NICK") 
		cmdNick(client, ss); 
	else if (command == "USER")
		cmdUser(client, ss);
	else if (command == "QUIT")
		client.setWasDisconnected(true); // Let the main loop handle it

	// --- 2. THE GATEKEEPER ---
	// If they try to do anything else (JOIN, MODE, etc.) before finishing NICK/USER/PASS
	else if (!client.getIsRegistered())
	{
		client.reply("451", ":You have not registered"); 
		return;
	}

	// --- 3. REGISTERED ONLY ---
	else if (command == "PRIVMSG")
		cmdPrivmsg(client, line, ss);
	else if (command == "MODE")
		cmdMode(client, ss);
	else if (command == "JOIN")
		cmdJoin(client, ss);
	// ... other commands ...
	else
		client.reply("421", command + " :Unknown command");

	// --- 4. THE WELCOME TRIGGER ---
	// Check if the 3 requirements are met to finish registration
	if (!client.getIsRegistered() && client.getHasPass() && !client.getNickname().empty() && !client.getUsername().empty()) 
	{
		client.setIsRegistered(true);
		client.reply("001", ":Welcome to the IRC Network " + client.getNickname());
		
		if (_debug)
			std::cout << "[LOG] Registration Complete for " << client.getNickname() << std::endl;
	}
}