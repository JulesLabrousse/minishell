/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:13:14 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:13:15 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	free_words(char **words)
{
	int	i;

	if (!words)
		return ;
	i = 0;
	while (words[i])
		free(words[i++]);
	free(words);
}

int	words_count(char **words)
{
	int	i;

	i = 0;
	if (!words)
		return (0);
	while (words[i])
		i++;
	return (i);
}

int	words_append(char ***words, char *word, t_expand_error *err)
{
	char	**new;
	int		len;
	int		i;

	len = words_count(*words);
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

int	words_flush(char ***words, char **current, t_expand_error *err)
{
	char	*new_current;

	if (!**current)
		return (0);
	if (words_append(words, *current, err))
		return (1);
	new_current = ft_strdup("");
	if (!new_current)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	*current = new_current;
	return (0);
}

int	words_split(t_wctx *ctx, const char *text, t_expand_error *err)
{
	const char	*start;
	char		*chunk;
	char		*tmp;

	while (*text)
	{
		if (*text == ' ' || *text == '\t' || *text == '\n')
		{
			if (words_flush(ctx->words, ctx->current, err))
				return (1);
			while (*text == ' ' || *text == '\t' || *text == '\n')
				text++;
		}
		else
		{
			start = text;
			while (*text && *text != ' ' && *text != '\t' && *text != '\n')
				text++;
			chunk = ft_substr(start, 0, text - start);
			if (!chunk)
				return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
			tmp = ft_strjoin(*ctx->current, chunk);
			free(chunk);
			if (!tmp)
				return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
			free(*ctx->current);
			*ctx->current = tmp;
		}
	}
	return (0);
}
