/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:39:07 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:39:08 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

int	lex_word_token(const char **input, t_token **token, t_lex_error *err)
{
	t_segment	*segments;

	segments = NULL;
	while (**input && **input != ' ' && **input != '\t'
		&& **input != '|' && **input != '<' && **input != '>')
	{
		if (**input == '\'' || **input == '"')
		{
			if (scan_quoted_segment(input, &segments, err))
				return (free_segments(segments), 1);
		}
		else
		{
			if (scan_unquoted_segment(input, &segments, err))
				return (free_segments(segments), 1);
		}
	}
	if (new_word_token(segments, token, err))
		return (free_segments(segments), 1);
	return (0);
}

int	lex_operator_token(const char **input, t_token **token, t_lex_error *err)
{
	t_operator_type	op;

	if (**input == '|')
	{
		op = OP_PIPE;
		(*input)++;
	}
	else if (**input == '<' && (*input)[1] == '<')
	{
		op = OP_HEREDOC;
		(*input) += 2;
	}
	else if (**input == '>' && (*input)[1] == '>')
	{
		op = OP_APPEND;
		(*input) += 2;
	}
	else if (**input == '<')
	{
		op = OP_IN;
		(*input)++;
	}
	else
	{
		op = OP_OUT;
		(*input)++;
	}
	if (new_operator_token(op, token, err))
		return (1);
	return (0);
}

int	lexer(const char *input, t_token **tokens, t_lex_error *err)
{
	t_token	*token;

	*tokens = NULL;
	while (*input)
	{
		while (*input == ' ' || *input == '\t')
			input++;
		if (*input == '\0')
			break ;
		token = NULL;
		if (*input == '|' || *input == '<' || *input == '>')
		{
			if (lex_operator_token(&input, &token, err))
				return (free_tokens(*tokens), *tokens = NULL, 1);
		}
		else
		{
			if (lex_word_token(&input, &token, err))
				return (free_tokens(*tokens), *tokens = NULL, 1);
		}
		append_token(tokens, token);
	}
	return (0);
}
