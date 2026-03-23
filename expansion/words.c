/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 23:21:04 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 23:21:06 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	words_append(char ***words, char *word, t_expand_error *err)
{
	char	**new;
	int		len;
	int		i;

	len = 0;
	if (*words)
		while ((*words)[len])
			len++;
	new = malloc(sizeof(char *) * (len + 2));
	if (!new)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	i = 0;
	while (i < len)
	{
		new[i] = (*words)[i];
		i++;
	}
	new[len] = word;
	new[len + 1] = NULL;
	free(*words);
	*words = new;
	return (0);
}

static int	words_flush_into(t_wctx *ctx, t_expand_error *err)
{
	char	*empty;

	if (!(*ctx->current)[0])
		return (0);
	if (words_append(ctx->words, *ctx->current, err))
		return (1);
	empty = ft_strdup("");
	if (!empty)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	*ctx->current = empty;
	return (0);
}

static int	words_split_chunk(t_wctx *ctx, const char **text,
		t_expand_error *err)
{
	const char	*start;
	char		*chunk;
	char		*tmp;

	start = *text;
	while (**text && **text != ' ' && **text != '\t' && **text != '\n')
		(*text)++;
	chunk = ft_substr(start, 0, *text - start);
	if (!chunk)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	tmp = ft_strjoin(*ctx->current, chunk);
	free(chunk);
	if (!tmp)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	free(*ctx->current);
	*ctx->current = tmp;
	return (0);
}

int	words_split(t_wctx *ctx, const char *text, t_expand_error *err)
{
	while (*text)
	{
		if (*text == ' ' || *text == '\t' || *text == '\n')
		{
			if (words_flush_into(ctx, err))
				return (1);
			while (*text == ' ' || *text == '\t' || *text == '\n')
				text++;
		}
		else if (words_split_chunk(ctx, &text, err))
			return (1);
	}
	return (0);
}
