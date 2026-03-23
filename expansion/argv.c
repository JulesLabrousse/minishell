/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:07:13 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:08:49 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

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
	if (current[0] || has_quoted)
	{
		if (words_append(words, current, err))
			return (free(current), 1);
	}
	else
		free(current);
	return (0);
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
