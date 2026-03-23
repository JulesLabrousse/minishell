/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:12:29 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:12:46 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	expand_redir_target(t_segment *segs, char **target,
		t_shell *shell, t_expand_error *err)
{
	char	**words;

	words = NULL;
	if (expand_word(segs, &words, shell, err))
		return (1);
	if (!words || !words[0] || words[1])
	{
		free_words(words);
		err->type = EXPAND_ERR_AMBIGUOUS_REDIR;
		err->word = NULL;
		return (1);
	}
	*target = words[0];
	free(words);
	return (0);
}

int	expand_redir(t_parse_redir *in, t_exp_redir **out,
		t_shell *shell, t_expand_error *err)
{
	if (new_exp_redir(in->type, out, err))
		return (1);
	if (in->type == OP_HEREDOC)
	{
		(*out)->fd = in->heredoc.fd;
		in->heredoc.fd = -1;
		return (0);
	}
	if (expand_redir_target(in->target, &(*out)->target, shell, err))
		return (free(*out), *out = NULL, 1);
	return (0);
}

int	expand_redirs(t_parse_redir *in, t_exp_redir **out,
		t_shell *shell, t_expand_error *err)
{
	t_exp_redir	*redir;
	t_exp_redir	*tail;

	*out = NULL;
	tail = NULL;
	while (in)
	{
		if (expand_redir(in, &redir, shell, err))
			return (free_exp_redirs(*out), *out = NULL, 1);
		if (!*out)
			*out = redir;
		else
			tail->next = redir;
		tail = redir;
		in = in->next;
	}
	return (0);
}
