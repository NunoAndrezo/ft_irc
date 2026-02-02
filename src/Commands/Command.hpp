/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:16:35 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/02 13:32:31 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "../Server/Server.hpp"

class Server;

class Command
{
	protected:
		Server *_server;
	public:
		Command(Server *server);
		virtual ~Command();
		
		virtual void execute() = 0; // Pure virtual function to be overridden by derived classes

};

#endif