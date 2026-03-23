/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:11:15 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:11:25 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static int	expand_status(int status, char **out, t_expand_error *err)
{
	char	*str;
	char	*tmp;

	str = ft_itoa(status);
	if (!str)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	tmp = ft_strjoin(*out, str);
	free(str);
	if (!tmp)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	free(*out);
	*out = tmp;
	return (0);
}

static int	expand_var(const char **pos, char **out, t_shell *shell,
		t_expand_error *err)
{
	const char	*start;
	char		*name;
	char		*tmp;
	t_env		*var;

	start = *pos;
	while (**pos && (ft_isalnum(**pos) || **pos == '_'))
		(*pos)++;
	name = ft_substr(start, 0, *pos - start);
	if (!name)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	var = shell->vars;
	while (var && ft_strncmp(var->name, name, ft_strlen(name) + 1))
		var = var->next;
	free(name);
	if (!var || !var->value)
		return (0);
	tmp = ft_strjoin(*out, var->value);
	if (!tmp)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	free(*out);
	*out = tmp;
	return (0);
}

static int	expand_dollar(const char **pos, char **out, t_shell *shell,
		t_expand_error *err)
{
	char	*tmp;

	(*pos)++;
	if (**pos == '?')
		return ((*pos)++, expand_status(shell->last_status, out, err));
	if (**pos && (ft_isalnum(**pos) || **pos == '_'))
		return (expand_var(pos, out, shell, err));
	tmp = ft_strjoin(*out, "$");
	if (!tmp)
		return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
	free(*out);
	*out = tmp;
	return (0);
}

int	expand_text(const char *text, char **out, t_shell *shell,
		t_expand_error *err)
{
	const char	*start;
	char		*chunk;
	char		*tmp;

	while (*text)
	{
		start = text;
		while (*text && *text != '$')
			text++;
		chunk = ft_substr(start, 0, text - start);
		if (!chunk)
			return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
		tmp = ft_strjoin(*out, chunk);
		free(chunk);
		if (!tmp)
			return (err->type = EXPAND_ERR_NOMEM, err->saved_errno = errno, 1);
		free(*out);
		*out = tmp;
		if (*text == '$' && expand_dollar(&text, out, shell, err))
			return (1);
	}
	return (0);
}
