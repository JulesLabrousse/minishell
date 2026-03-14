/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:38:44 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:38:45 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parse_word(t_token **current, t_arg **args, t_parse_error *err)
{
	t_segment	*segments;
	t_arg		*arg;

	segments = (*current)->segments;
	(*current)->segments = NULL;
	if (new_arg(segments, &arg, err))
		return (free_segments(segments), 1);
	append_arg(args, arg);
	*current = (*current)->next;
	return (0);
}

int	parse_redirections(t_token **current, t_redir **redirs, t_parse_error *err)
{
	t_segment	*segments;
	t_redir		*redir;

	if ((*current)->next == NULL)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = UNEXPECTED_NEWLINE, 1);
	if ((*current)->next->type != TOK_WORD)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = (t_unexpected_token)(*current)->next->operator, 1);
	segments = (*current)->next->segments;
	(*current)->next->segments = NULL;
	if (new_redir((*current)->operator, segments, &redir, err))
		return (free_segments(segments), 1);
	append_redir(redirs, redir);
	*current = (*current)->next->next;
	return (0);
}

int	parse_pipe(t_token **current, t_parse_error *err)
{
	if ((*current)->next == NULL)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = UNEXPECTED_NEWLINE, 1);
	if ((*current)->next->type == TOK_OPERATOR
		&& (*current)->next->operator == OP_PIPE)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = UNEXPECTED_PIPE, 1);
	*current = (*current)->next;
	return (0);
}

int	parser(t_token *tokens, t_command **commands, t_parse_error *err)
{
	t_token		*current;
	t_command	*command;
	t_redir		*redirs;
	t_arg		*args;

	*commands = NULL;
	current = tokens;
	redirs = NULL;
	args = NULL;
	while (current)
	{
		if (current->type == TOK_WORD)
		{
			if (parse_word(&current, &args, err))
				return (free_redirs(redirs), free_args(args), 1);
		}
		else if (current->type == TOK_OPERATOR && current->operator != OP_PIPE)
		{
			if (parse_redirections(&current, &redirs, err))
				return (free_redirs(redirs), free_args(args), 1);
		}
		else if (current->type == TOK_OPERATOR && current->operator == OP_PIPE)
		{
			if (args == NULL && redirs == NULL)
				return (err->type = PARSE_ERR_SYNTAX,
					err->unexpected = UNEXPECTED_PIPE,
					free_redirs(redirs), free_args(args), 1);
			if (parse_pipe(&current, err))
				return (free_redirs(redirs), free_args(args), 1);
			if (new_command(redirs, args, &command, err))
				return (free_redirs(redirs), free_args(args), 1);
			append_command(commands, command);
			redirs = NULL;
			args = NULL;
		}
	}
	if (args == NULL && redirs == NULL)
		return (0);
	if (new_command(redirs, args, &command, err))
		return (free_redirs(redirs), free_args(args), 1);
	append_command(commands, command);
	return (0);
}
