/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 04:23:38 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 04:23:40 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <stdbool.h>
# include <errno.h>
# include <stdio.h>
# include <readline/readline.h>

extern volatile sig_atomic_t	g_signal;

typedef struct s_env
{
	char			*name;
	char			*value;
	bool			exported;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*vars;
	int		last_status;
}	t_shell;

/* --- SIGNALS --- */

void	setup_interactive_signals(void);
void	setup_heredoc_signals(void);

/* --- UTILS --- */

int		ft_isalnum(int c);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_itoa(int n);

#endif
