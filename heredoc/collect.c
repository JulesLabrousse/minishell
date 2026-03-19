/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 02:03:12 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 04:19:13 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

static bool	is_delim_quoted(t_segment *segments)
{
	while (segments)
	{
		if (segments->quote != QU_NONE)
			return (true);
		segments = segments->next;
	}
	return (false);
}

static int	delim_to_str(t_segment *segments, char **result,
		t_heredoc_error *err)
{
	char	*tmp;

	*result = ft_strdup("");
	if (!*result)
		return (err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, 1);
	while (segments)
	{
		tmp = ft_strjoin(*result, segments->text);
		if (!tmp)
			return (free(*result),
				err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, 1);
		free(*result);
		*result = tmp;
		segments = segments->next;
	}
	return (0);
}

static int	collect_heredoc(t_redir *redir, t_shell *shell,
	t_heredoc_error *err)
{
	t_heredoc_ctx	ctx;

	ctx.fd = open_next_heredoc_fd(err);
	if (ctx.fd < 0)
		return (1);
	ctx.expand = !is_delim_quoted(redir->heredoc.delim);
	if (delim_to_str(redir->heredoc.delim, (char **)&ctx.delim, err))
		return (close(ctx.fd), 1);
	if (read_heredoc(&ctx, shell, err))
		return (free((char *)ctx.delim), close(ctx.fd), 1);
	redir->heredoc.fd = ctx.fd;
	free((char *)ctx.delim);
	return (0);
}

int	collect_heredocs(t_command *commands, t_shell *shell, t_heredoc_error *err)
{
	t_redir	*redir;

	setup_heredoc_signals();
	while (commands && g_signal == 0)
	{
		redir = commands->redirs;
		while (redir && g_signal == 0)
		{
			if (redir->type == OP_HEREDOC)
				if (collect_heredoc(redir, shell, err))
					return (setup_interactive_signals(), 1);
			redir = redir->next;
		}
		commands = commands->next;
	}
	setup_interactive_signals();
	return (0);
}
