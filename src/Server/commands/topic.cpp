/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: famendes <famendes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:08:00 by famendes          #+#    #+#             */
/*   Updated: 2026/02/13 16:08:00 by famendes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../Server.hpp"

void Server::cmdTopic(Client& client, std::stringstream& ss)
{
    std::string channelName;
    std::string topic;

    if (!(ss >> channelName >> topic))
    {
        client.reply(ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters");
        return;
    }
    Channel* channel = getChannelByName(channelName);
    if (!channel)
    {
        client.reply(ERR_NOSUCHCHANNEL, channelName + " :No such channel");
        return;
    }
    if (!channel->findMember(&client))
    {
        client.reply(ERR_NOTONCHANNEL, channelName + " :You are not on that channel");
        return;
    }
    if (!channel->isMemberOperator(&client))
    {
        client.reply(ERR_CHANOPRIVSNEEDED, channelName + " :You're not channel operator");
        return;
    }
        if (topic[0] != ':')
        {
            client.reply(ERR_NEEDMOREPARAMS, "TOPIC :Topic must start with ':'");
            return;
        }
        topic.erase(0, 1); // Remove leading ':' if present
        channel->setTopic(topic);
        std::cout << topic << std::endl;
        if (topic.empty())
            client.reply(RPL_NOTOPIC, channelName + " :No topic is set");
        else
            client.reply(RPL_TOPIC, channelName + " :" + topic);
}
