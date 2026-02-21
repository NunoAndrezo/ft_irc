/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:06:47 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/21 14:38:59 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdInvite(Client& client, std::stringstream& ss)
{
	// To be implemented
	// if user is registered/authentificated
	if (client.getHasPass() && client.getIsRegistered())
	{
		std::string targetNickname, channelName;
		Channel* channel;
		if (!(ss >> targetNickname >> channelName))
		{
			client.reply(ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters");
			return;
		}
		// if channel exists
		channel = Server::getChannelByName(channelName);
		if (channel == NULL)
		{
			client.reply(ERR_NOSUCHCHANNEL, "Incorrect channel name. Channels must start with '#'");
			return;
		}
		// if target user exists
		if (!Server::hasClient(targetNickname))
		{
			client.reply(ERR_NOSUCHNICK, targetNickname + " :No such nick");
			return;
		}
		// if inviter is on channel and can invite (see if channel needs invitation and if inviter is operator)
		if (!channel->findMember(&client))
		{
			client.reply(ERR_NOTONCHANNEL, channelName + " :You are not on that channel");
			return;
		}
		if (channel->getModes() & MD_INV && !channel->isMemberOperator(&client) ) // if channel is invite only and inviter is not operator
		{
			client.reply(ERR_CHANOPRIVSNEEDED, channelName + " :You do not have the privileges to invite users to that channel");
			return;
		}
		client.reply(RPL_INVITING, targetNickname + " " + channelName);
		channel->addToInviteList(targetNickname); // wont add if already in list, so no need to check if user is already invited
	}
	else
	{
		client.reply(ERR_NOTREGISTERED, ":You have not registered");
	}
}