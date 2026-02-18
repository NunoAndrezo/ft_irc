/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:34:31 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/18 13:00:06 by nuno             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdPrivmsg(Client& client, std::string line, std::stringstream& ss) // pq mandar o ss e tambem a linha toda?
{
// Verificações de segurança (Registo)
	if (!client.getIsRegistered())
	{
		client.reply(ERR_NOTREGISTERED, ":You have not registered");
		return;
	}
	std::string target;
	if (!(ss >> target))
	{
		client.reply(ERR_NORECIPIENT, ":No recipient given (PRIVMSG)");
		return;
	}

	// 2. Extrair a mensagem (tudo o que vem depois dos primeiros ':')
	size_t pos = line.find(":");
	if (pos == std::string::npos)
	{
		client.reply(ERR_NOTEXTTOSEND, ":No text to send");
		return;
	}
	
	std::string msg = line.substr(pos); // Mantém o ':' para o protocolo

	// 3. Tentar encontrar o alvo (Cliente ou Canal)
	Client* targetClient = getClientByNickname(target);
	Channel* targetChannel = getChannelByName(target);

	if (targetClient)
	{
		// Formato: :Nick!User@Host PRIVMSG Target :Mensagem
		std::string fullMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() 
							+ " PRIVMSG " + target + " " + msg + "\r\n";
		send(targetClient->getFd(), fullMsg.c_str(), fullMsg.length(), 0);
	}
	else if (targetChannel)
	{
		if (!targetChannel->isMember(&client)) // Se o gajo não está no canal, não pode falar!
		{
			client.reply(ERR_CANNOTSENDTOCHAN, target + " :Cannot send to channel");
			return;
		}
		// O broadcastMessage deve encarregar-se de formatar a prefixo corretamente
		targetChannel->broadcastMessage(msg, &client);
	}
	else
	{
		client.reply(ERR_NOSUCHNICK, target + " :No such nick/channel");
	}
}
