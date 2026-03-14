/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:07:53 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:07:54 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	append_segment(t_segment **head, t_segment *segment)
{
	t_segment	*current;

	if (!head || !segment)
		return ;
	segment->next = NULL;
	if (*head == NULL)
	{
		*head = segment;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = segment;
}

void	append_token(t_token **head, t_token *token)
{
	t_token	*current;

	if (!head || !token)
		return ;
	token->next = NULL;
	if (*head == NULL)
	{
		*head = token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = token;
}
