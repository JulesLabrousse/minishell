/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:35:06 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:35:07 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	append_arg(t_arg **head, t_arg *arg)
{
	t_arg	*current;

	if (!head || !arg)
		return ;
	arg->next = NULL;
	if (*head == NULL)
	{
		*head = arg;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = arg;
}

void	append_redir(t_parse_redir **head, t_parse_redir *redir)
{
	t_parse_redir	*current;

	if (!head || !redir)
		return ;
	redir->next = NULL;
	if (*head == NULL)
	{
		*head = redir;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = redir;
}

void	append_command(t_parse_command **head, t_parse_command *command)
{
	t_parse_command	*current;

	if (!head || !command)
		return ;
	command->next = NULL;
	if (*head == NULL)
	{
		*head = command;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = command;
}
