/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:15:15 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/21 14:56:11 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


int Server::parseCommand(const std::string& cmd) const
{
	if (cmd == "CAP") return CAP;
	if (cmd == "PASS") return PASS;
	if (cmd == "NICK") return NICK;
	if (cmd == "USER") return USER;
    if (cmd == "PRIVMSG") return PRIVMSG;
	if (cmd == "PING") return PING;
	if (cmd == "MODE") return MODE;
    if (cmd == "JOIN") return JOIN;
	if (cmd == "INVITE") return INVITE;
	if (cmd == "KICK") return KICK;
	if (cmd == "TOPIC") return TOPIC;
    if (cmd == "QUIT") return QUIT;

	return UNKNOWN;
}

void Server::processCommand(Client& client, std::string line)
{
	std::stringstream ss(line);
	std::string command;
	ss >> command;
	for (size_t i = 0; i < command.length(); i++)
		command[i] = toupper(command[i]);

	if (command.empty())
		return;
	switch (parseCommand(command)) 
	{
		case CAP:
			cmdCap(client);
			break;
		case PASS:
			cmdPass(client, ss);
			break;
		case NICK:
			cmdNick(client, ss);
			break;
		case USER:
			cmdUser(client, ss);
			break;
		case PRIVMSG:
			cmdPrivmsg(client, line, ss);
			break;
		case PING:
			client.reply("PONG", ":pong");
			break;
		case MODE:
			cmdMode(client, ss);
			break;
		case JOIN:
			cmdJoin(client, ss);
			break;
		case INVITE:
			cmdInvite(client, ss);
			break;
		case KICK:
			cmdKick(client, ss);
			break;
		case TOPIC:
			cmdTopic(client, ss);
			break;
		case QUIT:
			cmdQuit(client, ss);
			break;
		default:
			client.reply(ERR_UNKNOWNCOMMAND, command + " :Unknown command");
			break;
	}
	// FINAL REGISTRATION CHECK
	if (!client.getIsRegistered() && client.getHasPass() && !client.getNickname().empty() && !client.getUsername().empty()) 
	{
		client.setIsRegistered(true);
		client.reply(RPL_WELCOME, ":Welcome to the Internet Relay Network " + client.getNickname());
		std::cout << "[LOG] Client '" << client.getNickname() << "' (FD " << client.getFd() << ") successfully registered." << std::endl;
	}
}
