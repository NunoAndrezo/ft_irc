/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* join.cpp                                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: nuno <nuno@student.42.fr>                  +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/02/09 17:45:35 by toferrei          #+#    #+#             */
/* Updated: 2026/02/12 00:30:00 by nuno             ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdJoin(Client &client, std::stringstream &ss)
{
	if (!client.getIsRegistered()) {
		client.reply(ERR_NOTREGISTERED, ":You have not registered");
		return;
	}

	std::string channelName;
	if (!(ss >> channelName)) {
		client.reply(ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
		return;
	}

	if (channelName[0] != '#') {
		client.reply(ERR_NOSUCHCHANNEL, channelName + " :Incorrect channel name (# required)");
		return;
	}

	Channel* channel = getChannelByName(channelName);
	
	if (!channel) {
		channel = new Channel(channelName, _debug);
		_channels.push_back(channel);
		channel->addMember(&client);
		channel->setMemberAsOperator(&client);
		channel->removeMode(MD_INV); // Start open by default
	} 
	else {
		if (channel->findMember(&client)) return;

		// MODE: Invite-Only (+i)
		if ((channel->getModes() & MD_INV) && !channel->isInInviteList(client.getNickname())) {
			client.reply(ERR_INVITEONLYCHAN, channelName + " :Cannot join channel (+i)");
			return;
		}

		// MODE: Password Protected (+k) - THE FIX IS HERE
		if (channel->getModes() & MD_PASSWORD_PROT) {
			std::string providedPass;
			// If no password provided or it doesn't match, reject
			if (!(ss >> providedPass) || providedPass != channel->getPassword()) {
				client.reply(ERR_BADCHANNELKEY, channelName + " :Cannot join channel (+k)");
				return;
			}
		}

		// MODE: User Limit (+l)
		if ((channel->getModes() & MD_USR_LIM) && (int)channel->getMembers().size() >= channel->getUserLimit()) {
			client.reply(ERR_CHANNELISFULL, channelName + " :Cannot join channel (+l)");
			return;
		}

		channel->addMember(&client);
	}

	// BROADCAST JOIN
	std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + 
						"@" + client.getHostname() + " JOIN :" + channelName + "\r\n";
	channel->broadcastRawMessage(joinMsg);

	// RPL_TOPIC
	if (!channel->getTopic().empty())
		client.reply(RPL_TOPIC, channelName + " :" + channel->getTopic());

	// RPL_NAMREPLY & ENDOFNAMES
	client.reply(RPL_NAMREPLY, "= " + channelName + " :" + channel->getNamesList());
	client.reply(RPL_ENDOFNAMES, channelName + " :End of /NAMES list.");
}