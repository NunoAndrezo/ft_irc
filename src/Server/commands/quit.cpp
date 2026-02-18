/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:21:34 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/18 12:15:27 by nuno             ###   ########.fr       */
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
