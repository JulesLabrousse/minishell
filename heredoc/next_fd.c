/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 04:17:21 by jlabrous          #+#    #+#             */
/*   Updated: 2026/03/19 04:17:23 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heredoc.h"

static void	fill_hd_name(char *path, int n)
{
	path[0] = '/';
	path[1] = 't';
	path[2] = 'm';
	path[3] = 'p';
	path[4] = '/';
	path[5] = 'h';
	path[6] = 'd';
	path[7] = '_';
	path[8] = '0' + ((n / 100000) % 10);
	path[9] = '0' + ((n / 10000) % 10);
	path[10] = '0' + ((n / 1000) % 10);
	path[11] = '0' + ((n / 100) % 10);
	path[12] = '0' + ((n / 10) % 10);
	path[13] = '0' + (n % 10);
	path[14] = '\0';
}

int	open_next_heredoc_fd(t_heredoc_error *err)
{
	char	path[15];
	int		i;
	int		fd;

	i = 0;
	while (i <= 999999)
	{
		fill_hd_name(path, i);
		fd = open(path, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd >= 0)
			return (unlink(path), fd);
		if (errno == EEXIST)
			i++;
		else
			return (err->type = HEREDOC_ERR_OPEN,
				err->saved_errno = errno, -1);
	}
	return (err->type = HEREDOC_ERR_OPEN, err->saved_errno = EEXIST, -1);
}
