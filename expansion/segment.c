/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:09:41 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:10:17 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	expand_seg_quoted(t_segment *seg, char **current, t_shell *shell,
		t_expand_error *err)
{
	char	*tmp;

	if (seg->quote == QU_SINGLE)
	{
		tmp = ft_strjoin(*current, seg->text);
		if (!tmp)
			return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
		free(*current);
		*current = tmp;
		return (0);
	}
	return (expand_text(seg->text, current, shell, err));
}

static int	expand_seg_none(t_segment *seg, t_wctx *ctx, t_shell *shell,
		t_expand_error *err)
{
	char	*expanded;

	expanded = ft_strdup("");
	if (!expanded)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	if (expand_text(seg->text, &expanded, shell, err))
		return (free(expanded), 1);
	if (words_split(ctx, expanded, err))
		return (free(expanded), 1);
	free(expanded);
	return (0);
}

int	expand_segment(t_segment *seg, t_wctx *ctx, t_shell *shell,
		t_expand_error *err)
{
	if (seg->quote == QU_NONE)
		return (expand_seg_none(seg, ctx, shell, err));
	return (expand_seg_quoted(seg, ctx->current, shell, err));
}
