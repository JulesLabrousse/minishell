/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 03:51:10 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 03:51:11 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

static void	sigint_interactive(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

static void	sigint_heredoc(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
}

void	setup_interactive_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_interactive;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
