/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:15:15 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/09 20:09:41 by toferrei         ###   ########.fr       */
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

	if (command.empty())
		return;

	// 1. QUIT handled in main loop to ensure cleanup
	
	// 2. CAP
	if (command == "CAP")
	{
		cmdCap(client);
	}
	else if (command == "PASS")
	{
		cmdPass(client, ss);
	}
	else if (command == "NICK") 
	{
		cmdNick(client, ss);
	} 
	else if (command == "USER")
	{
		cmdUser(client, ss);
	}
	else if (command == "PRIVMSG")
	{
		cmdPrivmsg(client, line, ss);
	}
	else if (command == "PING")
	{
		client.reply("PONG", ":pong");
	}
	else if (command == "MODE")
	{
		cmdMode(client, ss);
	}
	else if (command == "JOIN")
	{
		cmdJoin(client, ss);
	}
	else if (command == "INVITE")
	{
		cmdInvite(client, ss);
	}
	else
	{
		client.reply(ERR_UNKNOWNCOMMAND, command + " :Unknown command");
	}

	// FINAL REGISTRATION CHECK
	if (!client.getIsRegistered() && client.getHasPass() && !client.getNickname().empty() && !client.getUsername().empty()) {
		client.setIsRegistered(true);
		client.reply(RPL_WELCOME, ":Welcome to the Internet Relay Network " + client.getNickname());
		std::cout << "[LOG] Client '" << client.getNickname() << "' (FD " << client.getFd() << ") successfully registered." << std::endl;
	}
}