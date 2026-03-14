/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:50:04 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 17:57:17 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lexing/lexing.h"
#include "parsing/parsing.h"

volatile sig_atomic_t	g_signal = 0;

static void	sigint_interactive(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

static void	setup_interactive_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_interactive;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static int	handle_line(char *line, int *last_status)
{
	t_token			*tokens;
	t_command		*commands;
	t_lex_error		lex_err;
	t_parse_error	parse_err;

	tokens = NULL;
	commands = NULL;
	if (lexer(line, &tokens, &lex_err))
	{
		if (lex_err.type == LEX_ERR_UNCLOSED_QUOTE)
			print_lex_error(lex_err.quote);
		else
			perror("minishell");
		*last_status = 2;
		return (1);
	}
	if (parser(tokens, &commands, &parse_err))
	{
		if (parse_err.type == PARSE_ERR_SYNTAX)
			print_parse_error(parse_err.unexpected);
		else
			perror("minishell");
		free_tokens(tokens);
		*last_status = 2;
		return (1);
	}
	free_commands(commands);
	free_tokens(tokens);
	return (0);
}

int	main(void)
{
	char	*line;
	int		last_status;

	last_status = 0;
	while (1)
	{
		setup_interactive_signals();
		line = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			last_status = 130;
			g_signal = 0;
		}
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*line != '\0')
			add_history(line);
		if (*line != '\0')
			handle_line(line, &last_status);
		free(line);
	}
	return (last_status);
}
