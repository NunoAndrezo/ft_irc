/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:22:30 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/04 16:24:35 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdPass(Client& client, std::stringstream& ss)
{
	if (client.getIsRegistered())
	{
		client.reply(ERR_ALREADYREGISTRED, ":You may not reregister");
		return;
	}
	std::string pass;
	ss >> pass;
	if (pass == _serverPassword)
	{
		client.setHasPass(true);
		client.reply(RPL_NOTICE, "Step 1/3: Password accepted. Now send NICK <nickname>.");
	}
	else
	{
		client.reply(ERR_PASSWDMISMATCH, ":Password incorrect. Disconnecting.");
		client.setWasDisconnected(true);
	}
}
