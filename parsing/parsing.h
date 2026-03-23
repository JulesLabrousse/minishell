/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 02:03:12 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 17:11:17 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include "../lexing/lexing.h"

typedef struct s_heredoc
{
	t_segment	*delim;
	int			fd;
}	t_heredoc;

typedef struct s_parse_redir
{
	t_operator_type	type;
	union
	{
		t_segment	*target;
		t_heredoc	heredoc;
	};
	struct s_parse_redir	*next;
}	t_parse_redir;

typedef struct s_arg
{
	t_segment		*segments;
	struct s_arg	*next;
}	t_arg;

typedef struct s_parse_command
{
	t_parse_redir		*redirs;
	t_arg				*args;
	struct s_parse_command	*next;
}	t_parse_command;

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

/* --- FREE --- */

void	free_args(t_arg *current);
void	free_redirs(t_parse_redir *current);
void	free_commands(t_parse_command *current);

/* --- APPEND --- */

void	append_arg(t_arg **head, t_arg *arg);
void	append_redir(t_parse_redir **head, t_parse_redir *redir);
void	append_command(t_parse_command **head, t_parse_command *command);

/* --- NEW --- */

int		new_arg(t_segment *segments, t_arg **arg, t_parse_error *err);
int		new_redir(t_operator_type type, t_segment *segments, t_parse_redir **redir,
			t_parse_error *err);
int		new_command(t_parse_redir *redirs, t_arg *args, t_parse_command **command,
			t_parse_error *err);

/* --- PARSER --- */

int		parse_word(t_token **current, t_arg **args, t_parse_error *err);
int		parse_redirections(t_token **current, t_parse_redir **redirs,
			t_parse_error *err);
int		parse_pipe(t_token **current, t_parse_error *err);
int		parser(t_token *tokens, t_parse_command **commands, t_parse_error *err);

/* --- ERROR --- */

void	print_parse_error(t_unexpected_token token);

#endif
