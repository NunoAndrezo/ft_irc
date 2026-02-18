/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:21:34 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/18 13:15:49 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdQuit(Client &c, std::stringstream &ss)
{
	std::string reason;
	std::getline(ss, reason);

	// Limpeza da string da razão
	if (!reason.empty() && reason[0] == ' ') reason.erase(0, 1);
	if (!reason.empty() && reason[0] == ':') reason.erase(0, 1);
	if (reason.empty()) reason = "Client Quit";

	// 1. avisaa todos os canais
	handleQuitLogic(c, reason);

	// 2. Ativa a flag. A função 'clientMessage' vai ver isto e chama disconnectClient
	c.setWasDisconnected(true);
}

void Server::handleQuitLogic(Client &c, std::string reason)
{
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ) {
		if ((*it)->isMember(&c))
		{
			(*it)->broadcast(":" + c.getNickname() + " QUIT :" + reason);
			(*it)->removeMember(&c);
			if ((*it)->getMemberCount() == 0) {
				delete *it;
				it = _channels.erase(it);
				continue;
			}
		}
		++it;
	}
}
