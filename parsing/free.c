/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:34:23 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:34:24 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_args(t_arg *current)
{
	t_arg	*next;

	while (current)
	{
		next = current->next;
		free_segments(current->segments);
		free(current);
		current = next;
	}
}

void	free_redirs(t_redir *current)
{
	t_redir	*next;

	while (current)
	{
		next = current->next;
		if (current->type == OP_HEREDOC)
		{
			free_segments(current->heredoc.delim);
			if (current->heredoc.fd >= 0)
				close(current->heredoc.fd);
		}
		else
			free_segments(current->target);
		free(current);
		current = next;
	}
}

void	free_commands(t_command *current)
{
	t_command	*next;

	while (current)
	{
		next = current->next;
		free_redirs(current->redirs);
		free_args(current->args);
		free(current);
		current = next;
	}
}
