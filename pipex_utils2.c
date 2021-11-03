/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:05:25 by sameye            #+#    #+#             */
/*   Updated: 2021/11/03 18:47:43 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	errorfree(t_pipex *p)
{
	freepipex(p);
	perror("Error");
	exit(EXIT_FAILURE);
}

void	ft_putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	write(fd, str, ft_strlen(str));
}

int	printerror(char *str1, char *str2, int err)
{
	ft_putstr_fd(str1, 1);
	ft_putstr_fd(str2, 1);
	ft_putstr_fd("\n", 1);
	return (err);
}
