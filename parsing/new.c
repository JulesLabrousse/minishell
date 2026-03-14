/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:35:48 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:35:49 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	new_arg(t_segment *segments, t_arg **arg, t_parse_error *err)
{
	*arg = NULL;
	*arg = malloc(sizeof(t_arg));
	if (!*arg)
	{
		err->type = PARSE_ERR_NOMEM;
		err->saved_errno = errno;
		return (1);
	}
	(*arg)->segments = segments;
	(*arg)->next = NULL;
	return (0);
}

int	new_redir(t_operator_type type, t_segment *segments, t_redir **redir,
		t_parse_error *err)
{
	*redir = NULL;
	*redir = malloc(sizeof(t_redir));
	if (!*redir)
	{
		err->type = PARSE_ERR_NOMEM;
		err->saved_errno = errno;
		return (1);
	}
	(*redir)->type = type;
	(*redir)->next = NULL;
	if (type == OP_HEREDOC)
	{
		(*redir)->heredoc.delim = segments;
		(*redir)->heredoc.fd = -1;
	}
	else
		(*redir)->target = segments;
	return (0);
}

int	new_command(t_redir *redirs, t_arg *args, t_command **command,
		t_parse_error *err)
{
	*command = NULL;
	*command = malloc(sizeof(t_command));
	if (!*command)
	{
		err->type = PARSE_ERR_NOMEM;
		err->saved_errno = errno;
		return (1);
	}
	(*command)->redirs = redirs;
	(*command)->args = args;
	(*command)->next = NULL;
	return (0);
}
