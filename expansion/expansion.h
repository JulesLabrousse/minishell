/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:18:44 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 17:12:04 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "heredoc/heredoc.h"

typedef struct s_redir
{
	t_operator_type	type;
	union
	{
		char	*target;
		int		fd;
	};
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				**argv;
	t_redir				*redirs;
	struct s_command	*next;
}	t_command;

#endif
