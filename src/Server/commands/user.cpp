/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:34:38 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/04 16:36:08 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdUser(Client& client, std::stringstream& ss)
{
	if (!client.getHasPass())
	{
		client.reply(ERR_NOTREGISTERED, ":You must send PASS first");
		return; 
	}
	if (client.getIsRegistered())
	{
		client.reply(ERR_ALREADYREGISTRED, ":You may not reregister");
		return;
	}
	std::string user, host, server, real;
	if (!(ss >> user >> host >> server))
	{
		client.reply(ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
	}
	else
	{
		size_t pos = ss.str().find(":");
		if (pos != std::string::npos)
		{
			real = ss.str().substr(pos + 1);
			client.setUsername(user);
			client.setRealname(real);
			client.reply(RPL_NOTICE, "Step 3/3: User info saved.");
		}
	}
}