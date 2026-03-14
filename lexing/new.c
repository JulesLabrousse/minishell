/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:08:28 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 17:54:22 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

int	new_segment(t_segment_data data, t_segment **segment, t_lex_error *err)
{
	size_t	i;

	*segment = NULL;
	*segment = malloc(sizeof(t_segment));
	if (!*segment)
		return (err->type = LEX_ERR_NOMEM, err->saved_errno = errno, 1);
	(*segment)->text = malloc(sizeof(char) * (data.len + 1));
	if (!(*segment)->text)
		return (free(*segment), *segment = NULL,
			err->type = LEX_ERR_NOMEM, err->saved_errno = errno, 1);
	i = 0;
	while (i < data.len)
	{
		(*segment)->text[i] = data.start[i];
		i++;
	}
	(*segment)->text[data.len] = '\0';
	(*segment)->quote = data.quote;
	(*segment)->next = NULL;
	return (0);
}

int	new_word_token(t_segment *segments, t_token **token, t_lex_error *err)
{
	*token = NULL;
	*token = malloc(sizeof(t_token));
	if (!*token)
		return (err->type = LEX_ERR_NOMEM, err->saved_errno = errno, 1);
	(*token)->type = TOK_WORD;
	(*token)->segments = segments;
	(*token)->next = NULL;
	return (0);
}

int	new_operator_token(t_operator_type operator, t_token **token,
	t_lex_error *err)
{
	*token = NULL;
	*token = malloc(sizeof(t_token));
	if (!*token)
		return (err->type = LEX_ERR_NOMEM, err->saved_errno = errno, 1);
	(*token)->type = TOK_OPERATOR;
	(*token)->operator = operator;
	(*token)->next = NULL;
	return (0);
}
