/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:05:23 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:05:24 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../heredoc/heredoc.h"

typedef struct s_exp_redir
{
	t_operator_type		type;
	union
	{
		char	*target;
		int		fd;
	};
	struct s_exp_redir	*next;
}	t_exp_redir;

typedef struct s_exp_command
{
	char					**argv;
	t_exp_redir				*redirs;
	struct s_exp_command	*next;
}	t_exp_command;

typedef enum e_expand_error_type
{
	EXPAND_ERR_NOMEM,
	EXPAND_ERR_AMBIGUOUS_REDIR,
}	t_expand_error_type;

typedef struct s_expand_error
{
	t_expand_error_type	type;
	union
	{
		int		saved_errno;
		char	*word;
	};
}	t_expand_error;

typedef struct s_wctx
{
	char	***words;
	char	**current;
}	t_wctx;

/* --- FREE --- */

void	free_exp_redirs(t_exp_redir *current);
void	free_exp_commands(t_exp_command *current);

/* --- NEW --- */

int		new_exp_redir(t_operator_type type, t_exp_redir **redir,
			t_expand_error *err);
int		new_exp_command(char **argv, t_exp_redir *redirs,
			t_exp_command **cmd, t_expand_error *err);

/* --- WORDS --- */

void	free_words(char **words);
int		words_count(char **words);
int		words_append(char ***words, char *word, t_expand_error *err);
int		words_flush(char ***words, char **current, t_expand_error *err);
int		words_split(t_wctx *ctx, const char *text, t_expand_error *err);

/* --- TEXT --- */

int		expand_text(const char *text, char **out,
			t_shell *shell, t_expand_error *err);

/* --- SEGMENT --- */

int		expand_segment(t_segment *seg, t_wctx *ctx,
			t_shell *shell, t_expand_error *err);

/* --- ARGV --- */

int		expand_word(t_segment *segs, char ***words,
			t_shell *shell, t_expand_error *err);
int		expand_argv(t_arg *args, char ***argv,
			t_shell *shell, t_expand_error *err);

/* --- REDIR --- */

int		expand_redir_target(t_segment *segs, char **target,
			t_shell *shell, t_expand_error *err);
int		expand_redir(t_parse_redir *in, t_exp_redir **out,
			t_shell *shell, t_expand_error *err);
int		expand_redirs(t_parse_redir *in, t_exp_redir **out,
			t_shell *shell, t_expand_error *err);

/* --- EXPAND --- */

int		expand_command(t_parse_command *in, t_exp_command **out,
			t_shell *shell, t_expand_error *err);
int		expand(t_parse_command *in, t_exp_command **out,
			t_shell *shell, t_expand_error *err);

/* --- ERROR --- */

void	print_expand_error(t_expand_error *err);

#endif
