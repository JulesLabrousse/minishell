/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:37:09 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:37:10 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_parse_error(t_unexpected_token token)
{
	if (token == UNEXPECTED_PIPE)
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
	else if (token == UNEXPECTED_IN)
		write(2, "minishell: syntax error near unexpected token `<'\n", 51);
	else if (token == UNEXPECTED_OUT)
		write(2, "minishell: syntax error near unexpected token `>'\n", 51);
	else if (token == UNEXPECTED_HEREDOC)
		write(2, "minishell: syntax error near unexpected token `<<'\n", 52);
	else if (token == UNEXPECTED_APPEND)
		write(2, "minishell: syntax error near unexpected token `>>'\n", 52);
	else
		write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
}
