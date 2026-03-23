/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 04:28:51 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 04:28:53 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include <fcntl.h>
# include "../minishell.h"
# include "../parsing/parsing.h"

typedef enum e_heredoc_error_type
{
	HEREDOC_ERR_NOMEM,
	HEREDOC_ERR_WRITE,
	HEREDOC_ERR_OPEN,
}	t_heredoc_error_type;

typedef struct s_heredoc_error
{
	t_heredoc_error_type	type;
	int						saved_errno;
}	t_heredoc_error;

typedef struct s_heredoc_ctx
{
	int			fd;
	const char	*delim;
	bool		expand;
}	t_heredoc_ctx;

/* --- COLLECT --- */

int		collect_heredocs(t_parse_command *commands, t_shell *shell,
			t_heredoc_error *err);

/* --- READ --- */

int		read_heredoc(t_heredoc_ctx *ctx, t_shell *shell, t_heredoc_error *err);

/* --- EXPAND --- */

char	*expand_heredoc_line(char *line, t_shell *shell, t_heredoc_error *err);

/* --- NEXT FD --- */

int		open_next_heredoc_fd(t_heredoc_error *err);

#endif
