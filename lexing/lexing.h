/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:59:45 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 17:59:52 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <stddef.h>

typedef enum e_token_type
{
	TOK_WORD,
	TOK_OPERATOR
}	t_token_type;

typedef enum e_operator_type
{
	OP_PIPE,
	OP_IN,
	OP_OUT,
	OP_HEREDOC,
	OP_APPEND
}	t_operator_type;

typedef enum e_quote_type
{
	QU_NONE = 0,
	QU_SINGLE = '\'',
	QU_DOUBLE = '"'
}	t_quote_type;

typedef struct s_segment
{
	char				*text;
	t_quote_type		quote;
	struct s_segment	*next;
}	t_segment;

typedef struct s_token
{
	t_token_type	type;
	union
	{
		t_segment		*segments;
		t_operator_type	operator;
	};
	struct s_token	*next;
}	t_token;

typedef enum e_lex_error_type
{
	LEX_ERR_UNCLOSED_QUOTE,
	LEX_ERR_NOMEM
}	t_lex_error_type;

typedef struct s_lex_error
{
	t_lex_error_type	type;
	union
	{
		t_quote_type	quote;
		int				err_no;
	};
}	t_lex_error;

/* --- FREE --- */

void	free_segments(t_segment *current);
void	free_tokens(t_token *current);

/* --- APPEND --- */

void	append_segment(t_segment **head, t_segment *segment);
void	append_token(t_token **head, t_token *token);

/* --- NEW --- */

typedef struct s_segment_data
{
	char			*start;
	size_t			len;
	t_quote_type	quote;
}	t_segment_data;

int		new_segment(t_segment_data data, t_segment **segment,
			t_lex_error *err);
int		new_word_token(t_segment *segments, t_token **token,
			t_lex_error *err);
int		new_operator_token(t_operator_type operator, t_token **token,
			t_lex_error *err);

/* --- SCAN --- */

int	scan_unquoted_segment(const char **input, t_segment **segments,
	t_lex_error *err);
int	scan_quoted_segment(const char **input, t_segment **segments,
	t_lex_error *err);

/* --- LEXER --- */

int		lex_word_token(const char **input, t_token **token,
			t_lex_error *err);
int		lex_operator_token(const char **input, t_token **token,
			t_lex_error *err);
int		lexer(const char *input, t_token **tokens, t_lex_error *err);

/* --- ERROR --- */

void	print_lex_error(t_quote_type quote);

#endif
