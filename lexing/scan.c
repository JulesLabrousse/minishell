/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:09:54 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:09:55 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

int	scan_unquoted_segment(const char **input, t_segment **segments,
	t_lex_error *err)
{
	t_segment_data	data;
	t_segment		*segment;
	const char		*start;

	start = *input;
	while (**input && **input != ' ' && **input != '\t'
		&& **input != '|' && **input != '<' && **input != '>'
		&& **input != '\'' && **input != '"')
		(*input)++;
	data.start = start;
	data.len = *input - start;
	data.quote = QU_NONE;
	if (data.len == 0)
		return (0);
	if (new_segment(data, &segment, err))
		return (1);
	append_segment(segments, segment);
	return (0);
}

int	scan_quoted_segment(const char **input, t_segment **segments,
	t_lex_error *err)
{
	t_segment_data	data;
	t_segment		*segment;
	t_quote_type	quote;
	const char		*start;

	if (**input == '\'')
		quote = QU_SINGLE;
	else
		quote = QU_DOUBLE;
	(*input)++;
	start = *input;
	while (**input && **input != (char)quote)
		(*input)++;
	if (**input == '\0')
		return (err->type = LEX_ERR_UNCLOSED_QUOTE, err->quote = quote, 1);
	data.start = start;
	data.len = *input - start;
	data.quote = quote;
	if (new_segment(data, &segment, err))
		return (1);
	append_segment(segments, segment);
	(*input)++;
	return (0);
}
