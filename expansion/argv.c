/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 23:22:41 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 23:22:43 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	word_commit(char ***words, char **current, bool has_quoted,
		t_expand_error *err)
{
	if ((*current)[0] || has_quoted)
	{
		if (words_append(words, *current, err))
			return (free(*current), *current = NULL, 1);
		return (0);
	}
	free(*current);
	*current = NULL;
	return (0);
}

int	expand_word(t_segment *segs, char ***words, t_shell *shell,
		t_expand_error *err)
{
	char	*current;
	bool	has_quoted;
	t_wctx	ctx;

	current = ft_strdup("");
	if (!current)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	has_quoted = false;
	ctx.words = words;
	ctx.current = &current;
	while (segs)
	{
		if (segs->quote != QU_NONE)
			has_quoted = true;
		if (expand_segment(segs, &ctx, shell, err))
			return (free(current), 1);
		segs = segs->next;
	}
	return (word_commit(words, &current, has_quoted, err));
}

int	expand_argv(t_arg *args, char ***argv, t_shell *shell, t_expand_error *err)
{
	char	**words;

	words = NULL;
	*argv = NULL;
	while (args)
	{
		if (expand_word(args->segments, &words, shell, err))
			return (free_words(words), 1);
		args = args->next;
	}
	*argv = words;
	return (0);
}
