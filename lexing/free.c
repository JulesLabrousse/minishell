/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:07:03 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:07:04 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	free_segments(t_segment *current)
{
	t_segment	*next;

	while (current)
	{
		next = current->next;
		free(current->text);
		free(current);
		current = next;
	}
}

void	free_tokens(t_token *current)
{
	t_token	*next;

	while (current)
	{
		next = current->next;
		if (current->type == TOK_WORD)
			free_segments(current->segments);
		free(current);
		current = next;
	}
}
