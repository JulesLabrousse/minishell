/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:24:42 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/14 15:24:43 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"

void	print_lex_error(t_quote_type quote)
{
	if (quote == QU_SINGLE)
		write(2, "minishell: unclosed single quote\n", 33);
	else if (quote == QU_DOUBLE)
		write(2, "minishell: unclosed double quote\n", 33);
	else
		write(2, "minishell: lexical error\n", 25);
}
