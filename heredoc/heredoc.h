/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 03:54:11 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 03:54:15 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include <stdbool.h>
# include <signal.h>
# include <errno.h>
# include <unistd.h>
# include <readline/readline.h>
# include "../libft/libft.h"
# include "../parsing/parsing.h"
# include "../shell/shell.h"

extern volatile sig_atomic_t	g_signal;

typedef enum e_heredoc_error_type
{
	HEREDOC_ERR_NOMEM,
	HEREDOC_ERR_WRITE,
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

/* --- SIGNALS --- */

void	setup_interactive_signals(void);
void	setup_heredoc_signals(void);

/* --- COLLECT --- */

int		collect_heredocs(t_command *commands, t_shell *shell,
			t_heredoc_error *err);
int		collect_heredoc(t_redir *redir, t_shell *shell, t_heredoc_error *err);

/* --- READ --- */

int		read_heredoc(t_heredoc_ctx *ctx, t_shell *shell, t_heredoc_error *err);

/* --- EXPAND --- */

char	*expand_heredoc_line(char *line, t_shell *shell, t_heredoc_error *err);

/* --- NEXT FD --- */

int		next_available_fd(t_heredoc_error *err);

#endif
