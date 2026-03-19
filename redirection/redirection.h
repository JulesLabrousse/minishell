/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 06:17:54 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 17:15:56 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "expansion/expansion.h"

typedef struct s_command
{
	char				**argv;
	int					in_fd;
	int					out_fd;
	struct s_command	*next;
}	t_command;

#endif
