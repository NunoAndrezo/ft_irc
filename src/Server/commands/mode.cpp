/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:34:15 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/10 14:35:44 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdMode(Client& client, std::stringstream& ss)
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
	std::string target, mode;
	ss >> target;
	ss >> mode;
	std::cout << "target: " << target << " mode: " << mode << std::endl;
	if (Server::hasClient(target))
	{
		client.setUserMode(0x0001); // example: set invisible mode
		client.reply(RPL_UMODEIS, ":+i"); // example: user is invisible
	}
	//  se for canal
	//fazer outras cenas
}