/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 04:28:21 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 04:28:22 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include "lexing/lexing.h"
#include "parsing/parsing.h"
#include "heredoc/heredoc.h"

volatile sig_atomic_t	g_signal = 0;

static int	handle_lex(char *line, t_token **tokens, t_shell *shell)
{
	t_lex_error	err;

	if (lexer(line, tokens, &err))
	{
		if (err.type == LEX_ERR_UNCLOSED_QUOTE)
			print_lex_error(err.quote);
		else
			perror("minishell");
		shell->last_status = 2;
		return (1);
	}
	return (0);
}

static int	handle_parse(t_token *tokens, t_command **commands, t_shell *shell)
{
	t_parse_error	err;

	if (parser(tokens, commands, &err))
	{
		if (err.type == PARSE_ERR_SYNTAX)
			print_parse_error(err.unexpected);
		else
			perror("minishell");
		free_tokens(tokens);
		shell->last_status = 2;
		return (1);
	}
	return (0);
}

static int	handle_collect(t_token *tokens, t_command *commands, t_shell *shell)
{
	t_heredoc_error	err;

	if (collect_heredocs(commands, shell, &err))
	{
		perror("minishell");
		free_commands(commands);
		free_tokens(tokens);
		shell->last_status = 1;
		return (1);
	}
	if (g_signal == SIGINT)
	{
		shell->last_status = 130;
		g_signal = 0;
		free_commands(commands);
		free_tokens(tokens);
		return (1);
	}
	return (0);
}

static int	handle_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = NULL;
	commands = NULL;
	if (handle_lex(line, &tokens, shell))
		return (1);
	if (handle_parse(tokens, &commands, shell))
		return (1);
	if (handle_collect(tokens, commands, shell))
		return (1);
	/* expansions + execution */
	free_commands(commands);
	free_tokens(tokens);
	return (0);
}

int	main(void)
{
	t_shell	shell;
	char	*line;

	shell.vars = NULL;
	shell.last_status = 0;
	while (1)
	{
		setup_interactive_signals();
		line = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			shell.last_status = 130;
			g_signal = 0;
		}
		if (line == NULL)
			return (write(1, "exit\n", 5), shell.last_status);
		if (*line != '\0')
		{
			add_history(line);
			handle_line(line, &shell);
		}
		free(line);
	}
}
