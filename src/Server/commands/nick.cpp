/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:25:38 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/10 14:41:19 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdNick(Client& client, std::stringstream& ss)
{
	if (!client.getHasPass())
	{
		client.reply(ERR_NOTREGISTERED, ":You must send PASS first");
		return; 
	}
	std::string nick;
	if (!(ss >> nick))
	{
		client.reply(ERR_NONICKNAMEGIVEN, ":No nickname given");
		return;
	}
	if (Server::hasClient(nick))
	{
		client.reply(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use");
		return;
	}
	client.setNickname(nick);
	client.reply(RPL_NEWNICK, "Nickname successfully changed to " + nick);
}
