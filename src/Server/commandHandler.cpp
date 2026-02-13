/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:15:15 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/13 16:19:16 by famendes         ###   ########.fr       */
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
	else if (command == "USER") // after this does user always need to be indetified and registered?
	{
		cmdUser(client, ss);
	}

// maybe do the registerer check here?

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
	else if (command == "KICK")
	{
		cmdKick(client, ss);
	}
	else if (command == "TOPIC")
	{
		cmdTopic(client, ss);
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
