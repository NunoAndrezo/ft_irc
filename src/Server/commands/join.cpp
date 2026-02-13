/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 17:45:35 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/10 12:30:11 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdJoin(Client &client, std::stringstream &ss)
{
	if (client.getHasPass() && client.getIsRegistered()) // client is indeed on server and has passed password
	{	
		std::string channelName, password;

		if (!(ss >> channelName))
		{
			client.reply(ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
			return;
		}
		if (channelName[0] != '#')
		{
			client.reply(ERR_NOSUCHCHANNEL, "Incorrect channel name. Channel names must start with '#'");
			return;
		}
		Channel* channel = getChannelByName(channelName);
		if (!channel) // no channel by that name was found create channel and add client as operator, by default channel is set to invite only,without any other restriction
		{
			Channel* newChannel = new Channel(channelName, _debug);
			newChannel->addMember(&client);
			newChannel->setMemberAsOperator(&client);
			_channels.push_back(newChannel);
			if (_debug)
				std::cout << FGRN("[LOG] Created channel ") << channelName << " and added " << client.getNickname() << " as operator." << std::endl;
		}
		else // see if client can join existing channel (password, invite only, user limit)
		{
			if (channel->findMember(&client))
			{
				client.reply(ERR_USERONCHANNEL, channelName + " :You are already on that channel");
				return;
			}
			if (channel->getModes() & MD_PASSWORD_PROT && !(ss >> password) && password != channel->getPassword())
			{
				client.reply(ERR_BADCHANNELKEY, channelName + " :Cannot join channel (incorrect password)");
				return;
			}
			if (channel->getModes() & MD_INV && !channel->isInInviteList(client.getNickname()))
			{
				client.reply(ERR_INVITEONLYCHAN, channelName + " :Cannot join channel (invite only)");
				return;
			}
			if (channel->getModes() & MD_USR_LIM && channel->getUserLimit() <= (int)channel->getMembers().size())
			{
				client.reply(ERR_CHANNELISFULL, channelName + " :Cannot join channel (channel is full)");
				return;
			}
			channel->addMember(&client);
			if (_debug)
				std::cout << FGRN("[LOG] Added ") << client.getNickname() << " to channel " << channelName << "." << std::endl;
			std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + 
						"@" + client.getHostname() + " JOIN :" + channelName + "\r\n";
			channel->broadcastRawMessage(joinMsg);
		}
	}
	else
	{
		client.reply(ERR_NOTREGISTERED, ":You have not registered");
	}
}