/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 22:14:22 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/23 22:14:35 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	print_expand_error(t_expand_error *err)
{
	if (err->type == EXPAND_ERR_AMBIGUOUS_REDIR)
	{
		write(2, "minishell: ", 11);
		if (err->word)
		{
			write(2, err->word, ft_strlen(err->word));
			write(2, ": ", 2);
		}
		write(2, "ambiguous redirect\n", 19);
	}
	else
		perror("minishell");
}
