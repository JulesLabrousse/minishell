/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 15:35:04 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/13 19:39:23 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

typedef enum e_unexpected_token
{
	UNEXPECTED_PIPE = OP_PIPE,
	UNEXPECTED_IN = OP_IN,
	UNEXPECTED_OUT = OP_OUT,
	UNEXPECTED_HEREDOC = OP_HEREDOC,
	UNEXPECTED_APPEND = OP_APPEND,
	UNEXPECTED_NEWLINE
}	t_unexpected_token;

typedef enum e_parse_error_type
{
	PARSE_ERR_SYNTAX,
	PARSE_ERR_NOMEM
}	t_parse_error_type;

typedef struct s_parse_error
{
	t_parse_error_type	type;
	union
	{
		t_unexpected_token	unexpected;
		int					saved_errno;
	};
}	t_parse_error;

/* --- PARSING DATA --- */

typedef struct s_heredoc
{
	t_segment	*delim;
	int			fd;
}	t_heredoc;

typedef struct s_redir
{
	t_operator_type	type;
	union
	{
		t_segment	*target;
		t_heredoc	heredoc;
	};
	struct s_redir	*next;
}	t_redir;

typedef struct s_arg
{
	t_segment		*segments;
	struct s_arg	*next;
}	t_arg;

typedef struct s_command
{
	t_redir				*redirs;
	t_arg				*args;
	struct s_command	*next;
}	t_command;

/* --- FREE --- */

void	free_args(t_arg *current)
{
	t_arg	*next;

	while (current)
	{
		next = current->next;
		free_segments(current->segments);
		free(current);
		current = next;
	}
}

void	free_redirs(t_redir *current)
{
	t_redir	*next;

	while (current)
	{
		next = current->next;
		if (current->type == OP_HEREDOC)
		{
			free_segments(current->heredoc.delim);
			if (current->heredoc.fd >= 0)
				close(current->heredoc.fd);
		}
		else
			free_segments(current->target);
		free(current);
		current = next;
	}
}

void	free_commands(t_command *current)
{
	t_command	*next;

	while (current)
	{
		next = current->next;
		free_redirs(current->redirs);
		free_args(current->args);
		free(current);
		current = next;
	}
}

/* --- APPEND --- */

void	append_arg(t_arg **head, t_arg *arg)
{
	t_arg	*current;

	if (!head || !arg)
		return ;
	arg->next = NULL;
	if (*head == NULL)
	{
		*head = arg;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = arg;
}

void	append_redir(t_redir **head, t_redir *redir)
{
	t_redir	*current;

	if (!head || !redir)
		return ;
	redir->next = NULL;
	if (*head == NULL)
	{
		*head = redir;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = redir;
}

void	append_command(t_command **head, t_command *command)
{
	t_command	*current;

	if (!head || !command)
		return ;
	command->next = NULL;
	if (*head == NULL)
	{
		*head = command;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = command;
}

/* --- NEW --- */

int	new_arg(t_segment *segments, t_arg **arg, t_parse_error *err)
{
	*arg = NULL;
	*arg = malloc(sizeof(t_arg));
	if (!*arg)
	{
		err->type = PARSE_ERR_NOMEM;
		err->saved_errno = errno;
		return (1);
	}
	(*arg)->segments = segments;
	(*arg)->next = NULL;
	return (0);
}

int	new_redir(t_operator_type type, t_segment *segments, t_redir **redir,
		t_parse_error *err)
{
	*redir = NULL;
	*redir = malloc(sizeof(t_redir));
	if (!*redir)
	{
		err->type = PARSE_ERR_NOMEM;
		err->saved_errno = errno;
		return (1);
	}
	(*redir)->type = type;
	(*redir)->next = NULL;
	if (type == OP_HEREDOC)
	{
		(*redir)->heredoc.delim = segments;
		(*redir)->heredoc.fd = -1;
	}
	else
		(*redir)->target = segments;
	return (0);
}

int	new_command(t_redir *redirs, t_arg *args, t_command **command,
		t_parse_error *err)
{
	*command = NULL;
	*command = malloc(sizeof(t_command));
	if (!*command)
	{
		err->type = PARSE_ERR_NOMEM;
		err->saved_errno = errno;
		return (1);
	}
	(*command)->redirs = redirs;
	(*command)->args = args;
	(*command)->next = NULL;
	return (0);
}

/* --- PARSER --- */

int	parse_word(t_token **current, t_arg **args, t_parse_error *err)
{
	t_segment	*segments;
	t_arg		*arg;

	segments = (*current)->segments;
	(*current)->segments = NULL;
	if (new_arg(segments, &arg, err))
		return (free_segments(segments), 1);
	append_arg(args, arg);
	*current = (*current)->next;
	return (0);
}

int	parse_redirections(t_token **current, t_redir **redirs, t_parse_error *err)
{
	t_segment	*segments;
	t_redir		*redir;

	if ((*current)->next == NULL)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = UNEXPECTED_NEWLINE, 1);
	if ((*current)->next->type != TOK_WORD)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = (t_unexpected_token)(*current)->next->operator, 1);
	segments = (*current)->next->segments;
	(*current)->next->segments = NULL;
	if (new_redir((*current)->operator, segments, &redir, err))
		return (free_segments(segments), 1);
	append_redir(redirs, redir);
	*current = (*current)->next->next;
	return (0);
}

int	parse_pipe(t_token **current, t_parse_error *err)
{
	if ((*current)->next == NULL)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = UNEXPECTED_NEWLINE, 1);
	if ((*current)->next->type == TOK_OPERATOR
		&& (*current)->next->operator == OP_PIPE)
		return (err->type = PARSE_ERR_SYNTAX,
			err->unexpected = UNEXPECTED_PIPE, 1);
	*current = (*current)->next;
	return (0);
}

int	parser(t_token *tokens, t_command **commands, t_parse_error *err)
{
	t_token		*current;
	t_command	*command;
	t_redir		*redirs;
	t_arg		*args;

	*commands = NULL;
	current = tokens;
	redirs = NULL;
	args = NULL;
	while (current)
	{
		if (current->type == TOK_WORD)
		{
			if (parse_word(&current, &args, err))
				return (free_redirs(redirs), free_args(args), 1);
		}
		else if (current->type == TOK_OPERATOR && current->operator != OP_PIPE)
		{
			if (parse_redirections(&current, &redirs, err))
				return (free_redirs(redirs), free_args(args), 1);
		}
		else if (current->type == TOK_OPERATOR && current->operator == OP_PIPE)
		{
			if (args == NULL && redirs == NULL)
				return (err->type = PARSE_ERR_SYNTAX,
					err->unexpected = UNEXPECTED_PIPE,
					free_redirs(redirs), free_args(args), 1);
			if (parse_pipe(&current, err))
				return (free_redirs(redirs), free_args(args), 1);
			if (new_command(redirs, args, &command, err))
				return (free_redirs(redirs), free_args(args), 1);
			append_command(commands, command);
			redirs = NULL;
			args = NULL;
		}
	}
	if (args == NULL && redirs == NULL)
		return (0);
	if (new_command(redirs, args, &command, err))
		return (free_redirs(redirs), free_args(args), 1);
	append_command(commands, command);
	return (0);
}

/* --- AFFICHAGE ERREUR PARSING --- */

void	print_parse_error(t_unexpected_token token)
{
	if (token == UNEXPECTED_PIPE)
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
	else if (token == UNEXPECTED_IN)
		write(2, "minishell: syntax error near unexpected token `<'\n", 51);
	else if (token == UNEXPECTED_OUT)
		write(2, "minishell: syntax error near unexpected token `>'\n", 51);
	else if (token == UNEXPECTED_HEREDOC)
		write(2, "minishell: syntax error near unexpected token `<<'\n", 52);
	else if (token == UNEXPECTED_APPEND)
		write(2, "minishell: syntax error near unexpected token `>>'\n", 52);
	else
		write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
}

/* appel cote main :

	if (parser(tokens, &commands, &err))
	{
		if (err.type == PARSE_ERR_SYNTAX)
			print_parse_error(err.unexpected);
		else if (err.type == PARSE_ERR_NOMEM)
		{
			errno = err.saved_errno;
			perror("minishell");
		}
		free_tokens(tokens);
		free_commands(commands);
		return ;
	}
	free_tokens(tokens);
*/