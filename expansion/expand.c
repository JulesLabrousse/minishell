/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 23:24:57 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 23:24:58 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	expand_command(t_parse_command *in, t_exp_command **out,
		t_shell *shell, t_expand_error *err)
{
	char		**argv;
	t_exp_redir	*redirs;

	argv = NULL;
	redirs = NULL;
	if (expand_argv(in->args, &argv, shell, err))
		return (1);
	if (expand_redirs(in->redirs, &redirs, shell, err))
		return (free_words(argv), 1);
	if (new_exp_command(argv, redirs, out, err))
		return (free_words(argv), free_exp_redirs(redirs), 1);
	return (0);
}

int	expand(t_parse_command *in, t_exp_command **out,
		t_shell *shell, t_expand_error *err)
{
	t_exp_command	*cmd;
	t_exp_command	*tail;

	*out = NULL;
	tail = NULL;
	while (in)
	{
		if (expand_command(in, &cmd, shell, err))
			return (free_exp_commands(*out), *out = NULL, 1);
		if (!*out)
			*out = cmd;
		else
			tail->next = cmd;
		tail = cmd;
		in = in->next;
	}
	return (0);
}
