/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 22:53:09 by nuno              #+#    #+#             */
/*   Updated: 2026/02/21 15:09:59 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::cmdMode(Client& client, std::stringstream& ss)
{
	std::string target, modeString;
	if (!(ss >> target)) 
	{
		client.reply(ERR_NEEDMOREPARAMS, "MODE :Not enough parameters");
		return;
	}

	if (target[0] != '#') 
	{
		if (target != client.getNickname()) {
			client.reply(ERR_USERSDONTMATCH, ":Cannot change mode for other users");
			return;
		}
		if (!(ss >> modeString)) {
			client.reply(RPL_UMODEIS, "+i");
			return;
		}
		return;
	}

	Channel* channel = getChannelByName(target);
	if (!channel) 
	{
		client.reply(ERR_NOSUCHCHANNEL, target + " :No such channel");
		return;
	}

	// Handle Query
	if (!(ss >> modeString)) 
	{
		std::string currentModes = "+";
		if (channel->getModes() & MD_INV) currentModes += "i";
		if (channel->getModes() & MD_TOPIC) currentModes += "t";
		if (channel->getModes() & MD_PASSWORD) currentModes += "k";
		if (channel->getModes() & MD_USR_LIM) currentModes += "l";
		
		std::string modeArgs = "";
		if (channel->getModes() & MD_PASSWORD) modeArgs += " " + channel->getPassword();
		if (channel->getModes() & MD_USR_LIM) 
		{
			std::stringstream oss;
			oss << channel->getUserLimit();
			modeArgs += " " + oss.str();
		}
		client.reply(RPL_CHANNELMODEIS, channel->getName() + " " + currentModes + modeArgs);
		return;
	}

	if (!channel->isMemberOperator(&client)) 
	{
		client.reply(ERR_CHANOPRIVSNEEDED, target + " :You're not channel operator");
		return;
	}

	bool adding = true;
	std::string appliedModes = "";
	std::string appliedArgs = "";
	std::string modeArg;

	for (size_t i = 0; i < modeString.length(); i++) 
	{
		char c = modeString[i];
		if (c == '+') { adding = true; continue; }
		if (c == '-') { adding = false; continue; }

		switch (c) 
		{
			case 'i':
				adding ? channel->addMode(MD_INV) : channel->removeMode(MD_INV);
				appliedModes += (adding ? "+i" : "-i");
				break;
			case 't':
				adding ? channel->addMode(MD_TOPIC) : channel->removeMode(MD_TOPIC);
				appliedModes += (adding ? "+t" : "-t");
				break;
			case 'k':
				if (adding && (ss >> modeArg)) 
				{
					channel->setPassword(modeArg);
					channel->addMode(MD_PASSWORD);
					appliedModes += "+k";
					appliedArgs += " " + modeArg;
				} 
				else if (!adding) 
				{
					channel->setPassword("");
					channel->removeMode(MD_PASSWORD);
					appliedModes += "-k";
				}
				break;
			case 'l':
				if (adding && (ss >> modeArg)) 
				{
					channel->setUserLimit(std::atoi(modeArg.c_str()));
					channel->addMode(MD_USR_LIM);
					appliedModes += "+l";
					appliedArgs += " " + modeArg;
				} 
				else if (!adding) 
				{
					channel->removeMode(MD_USR_LIM);
					appliedModes += "-l";
				}
				break;
			case 'o':
				if (ss >> modeArg) 
				{
					Client* targetClient = getClientByNickname(modeArg);
					if (targetClient && channel->findMember(targetClient)) 
					{
						adding ? channel->setMemberAsOperator(targetClient) : channel->unsetMemberAsOperator(targetClient);
						appliedModes += (adding ? "+o" : "-o");
						appliedArgs += " " + modeArg;
					} 
					else 
						client.reply(ERR_NOSUCHNICK, modeArg + " :No such nick");
				}
				break;
			default:
				client.reply(ERR_UMODEUNKNOWNFLAG, std::string("MODE :Unknown mode flag"));
				return;
		}
	}
	if (!appliedModes.empty()) 
	{
		std::string broadcastMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() 
								+ " MODE " + target + " " + appliedModes + appliedArgs + "\r\n";
		channel->broadcastRawMessage(broadcastMsg);
	}
}