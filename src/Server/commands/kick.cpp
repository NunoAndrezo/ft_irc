/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 21:17:13 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/10 15:37:07 by toferrei         ###   ########.fr       */
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
			client.reply(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters \"#channel target [reason]\"");
			return;
		}
		std::cout << "KICK command: channel=" << channelName << ", target=" << targetNickname << ", reason=" << reason << std::endl;
		Channel* channel = Server::getChannelByName(channelName);
		if (channel == NULL) //	 check if channel exists
		{
			client.reply(ERR_NOSUCHCHANNEL, channelName + " :No such channel");
			return;
		}
		std::cout << "01" << std::endl;
		if (!channel->findMember(&client)) // check if user is on the channel
		{
			client.reply(ERR_NOTONCHANNEL, channelName + " :You are not on that channel");
			return;
		}
		std::cout << "02" << std::endl;
		if (!channel->isMemberOperator(&client)) // only channel operators can kick users from channel
		{
			client.reply(ERR_CHANOPRIVSNEEDED, channelName + " :You do not have the privileges to kick users from that channel");
			return;
		}
		std::cout << "03" << std::endl;
		Client* targetClient = Server::getClientByNickname(targetNickname);
		std::cout << "04" << std::endl;
		if (targetClient == NULL) // check if target user exists
		{
			client.reply(ERR_NOSUCHNICK, targetNickname + " :No such nick");
			return;
		}
		std::cout << "05" << std::endl;
		if (!channel->findMember(targetClient)) // check if target user is on the channel
		{
			client.reply(ERR_USERNOTINCHANNEL, targetNickname + " " + channelName + " :That user is not on that channel");
			return;
		}
		std::cout << "06" << std::endl;
		std::string kickMsg = ":" + client.getNickname() + " KICK " + channelName + " " + targetNickname + " " + reason + CRLF;
		channel->broadcastMessage(kickMsg, &client); // broadcast kick message to all channel members
		channel->removeMember(targetClient); // remove target client from channel
		channel->removeFromInviteList(targetClient->getNickname()); // remove target client from invite list in case they were invited
		targetClient->reply("KICK", channelName + " :" + reason); // send kick message to target client
	}
	else
	{
		client.reply(ERR_NOTREGISTERED, ":You have not registered");
	}
}
