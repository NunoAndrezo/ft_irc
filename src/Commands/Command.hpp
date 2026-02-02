/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toferrei <toferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:16:35 by toferrei          #+#    #+#             */
/*   Updated: 2026/02/02 12:16:48 by toferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

class Command
{
	public:
		Command();
		virtual ~Command();
		
		virtual void execute() = 0; // Pure virtual function to be overridden by derived classes

};

#endif