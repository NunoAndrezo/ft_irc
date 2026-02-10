/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:34:31 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/10 12:53:37 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdPrivmsg(Client& client, std::string line, std::stringstream& ss) // pq mandar o ss e tambem a linha toda?
{
	if (!client.getHasPass())
	{
		client.reply(ERR_NOTREGISTERED, ":You must send PASS first");
		return; 
	}
	if (!client.getIsRegistered())
	{
		client.reply(ERR_NOTREGISTERED, ":You have not registered");
		return;
	}
	std::string target;
	ss >> target;
	size_t pos = line.find(":");
	if (pos != std::string::npos) // nn sei se faz sentido, nao deveria haver : de qualquer das formas
	{
		std::string msg = line.substr(pos);
		if (Server::hasClient(target))
		{
			std::string fullMsg = ":" + client.getNickname() + " PRIVMSG " + target + " " + msg + "\r\n";
			send(Server::getClientByNickname(target)->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
		}
		else if (Server::getChannelByName(target)) // target is a channel
		{
			Server::getChannelByName(target)->broadcastMessage(msg, &client);
		}
		else
			client.reply(ERR_NOSUCHNICK, target + " :No such nick");
	}
}
