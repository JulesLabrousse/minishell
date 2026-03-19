/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 03:55:22 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 03:55:24 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

static int	process_heredoc_line(t_heredoc_ctx *ctx, char *line,
		t_shell *shell, t_heredoc_error *err)
{
	char	*expanded;

	if (!ctx->expand)
	{
		if (write(ctx->fd, line, ft_strlen(line)) < 0
			|| write(ctx->fd, "\n", 1) < 0)
			return (err->type = HEREDOC_ERR_WRITE, err->saved_errno = errno, 1);
		return (0);
	}
	expanded = expand_heredoc_line(line, shell, err);
	if (!expanded)
		return (1);
	if (write(ctx->fd, expanded, ft_strlen(expanded)) < 0
		|| write(ctx->fd, "\n", 1) < 0)
		return (free(expanded),
			err->type = HEREDOC_ERR_WRITE, err->saved_errno = errno, 1);
	free(expanded);
	return (0);
}

int	read_heredoc(t_heredoc_ctx *ctx, t_shell *shell, t_heredoc_error *err)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal)
			return (0);
		if (!line)
		{
			write(2, "minishell: warning: here-document delimited ", 44);
			write(2, "by end-of-file (wanted `", 24);
			write(2, ctx->delim, ft_strlen(ctx->delim));
			write(2, "')\n", 3);
			return (0);
		}
		if (!ft_strncmp(line, ctx->delim, ft_strlen(ctx->delim) + 1))
			return (free(line), 0);
		if (process_heredoc_line(ctx, line, shell, err))
			return (free(line), 1);
		free(line);
	}
}
