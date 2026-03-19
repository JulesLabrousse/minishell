/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 03:55:50 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 03:55:52 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

static int	expand_status(int status, char **result, t_heredoc_error *err)
{
	char	*str;
	char	*tmp;

	str = ft_itoa(status);
	if (!str)
		return (err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, 1);
	tmp = ft_strjoin(*result, str);
	free(str);
	if (!tmp)
		return (err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, 1);
	free(*result);
	*result = tmp;
	return (0);
}

static int	expand_var(char **pos, t_shell *shell, char **result,
		t_heredoc_error *err)
{
	char	*start;
	char	*name;
	char	*tmp;
	t_env	*var;

	start = *pos;
	while (**pos && (ft_isalnum(**pos) || **pos == '_'))
		(*pos)++;
	name = ft_substr(start, 0, *pos - start);
	if (!name)
		return (err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, 1);
	var = shell->vars;
	while (var && ft_strncmp(var->name, name, ft_strlen(name) + 1))
		var = var->next;
	free(name);
	if (!var || !var->value)
		return (0);
	tmp = ft_strjoin(*result, var->value);
	if (!tmp)
		return (err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, 1);
	free(*result);
	*result = tmp;
	return (0);
}

static int	expand_dollar(char **pos, t_shell *shell, char **result,
		t_heredoc_error *err)
{
	char	*tmp;

	(*pos)++;
	if (**pos == '?')
		return ((*pos)++, expand_status(shell->last_status, result, err));
	if (**pos != '\0' && (ft_isalnum(**pos) || **pos == '_'))
		return (expand_var(pos, shell, result, err));
	tmp = ft_strjoin(*result, "$");
	if (!tmp)
		return (err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, 1);
	free(*result);
	*result = tmp;
	return (0);
}

char	*expand_heredoc_line(char *line, t_shell *shell, t_heredoc_error *err)
{
	char	*result;
	char	*start;
	char	*chunk;
	char	*tmp;

	result = ft_strdup("");
	if (!result)
		return (err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, NULL);
	while (*line)
	{
		start = line;
		while (*line && *line != '$')
			line++;
		chunk = ft_substr(start, 0, line - start);
		tmp = ft_strjoin(result, chunk);
		if (!chunk || !tmp)
			return (free(chunk), free(result),
				err->type = HEREDOC_ERR_NOMEM, err->saved_errno = errno, NULL);
		free(result);
		free(chunk);
		result = tmp;
		if (*line == '$' && expand_dollar(&line, shell, &result, err))
			return (free(result), NULL);
	}
	return (result);
}
