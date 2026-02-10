/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 21:17:13 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/10 12:47:10 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdKick(Client& client, std::stringstream& ss)
{
	// to be implemented --> check if user is registered/authentificated
	if (client.getHasPass() && client.getIsRegistered())
	{
		std::string channelName, targetNickname, reason;
		if (!(ss >> channelName >> targetNickname >> reason))
		{
			client.reply(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters");
			return;
		}
		std::cout << "KICK command: channel=" << channelName << ", target=" << targetNickname << ", reason=" << reason << std::endl;
		
	}
	else
	{
		client.reply(ERR_NOTREGISTERED, ":You have not registered");
	}
}
