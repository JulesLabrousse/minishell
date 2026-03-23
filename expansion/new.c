/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:13:38 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:13:41 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	new_exp_redir(t_operator_type type, t_exp_redir **redir,
		t_expand_error *err)
{
	*redir = malloc(sizeof(t_exp_redir));
	if (!*redir)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	(*redir)->type = type;
	(*redir)->next = NULL;
	if (type == OP_HEREDOC)
		(*redir)->fd = -1;
	else
		(*redir)->target = NULL;
	return (0);
}

int	new_exp_command(char **argv, t_exp_redir *redirs,
		t_exp_command **cmd, t_expand_error *err)
{
	*cmd = malloc(sizeof(t_exp_command));
	if (!*cmd)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	(*cmd)->argv = argv;
	(*cmd)->redirs = redirs;
	(*cmd)->next = NULL;
	return (0);
}
