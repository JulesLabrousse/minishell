/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 23:22:00 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 23:22:01 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	free_words(char **words)
{
	int	i;

	if (!words)
		return ;
	i = 0;
	while (words[i])
		free(words[i++]);
	free(words);
}

void	free_exp_redirs(t_exp_redir *current)
{
	t_exp_redir	*next;

	while (current)
	{
		next = current->next;
		if (current->type == OP_HEREDOC)
		{
			if (current->fd >= 0)
				close(current->fd);
		}
		else
			free(current->target);
		free(current);
		current = next;
	}
}

void	free_exp_commands(t_exp_command *current)
{
	t_exp_command	*next;

	while (current)
	{
		next = current->next;
		free_exp_redirs(current->redirs);
		free_words(current->argv);
		free(current);
		current = next;
	}
}
