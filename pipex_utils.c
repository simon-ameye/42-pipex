/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:10:11 by sameye            #+#    #+#             */
/*   Updated: 2021/10/25 16:35:32 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	printerror2(char *str1, char *str2, int err)
{
	ft_putstr_fd(str1, 1);
	ft_putstr_fd(str2, 1);
	ft_putstr_fd("\n", 1);
	return (err);
}

void	freetab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	initpipex(t_pipex *p)
{
	p->cmd1 = NULL;
	p->cmd2 = NULL;
	p->path1 = NULL;
	p->path2 = NULL;
	return (EXIT_SUCCESS);
}

int	freepipex(t_pipex *p)
{
	if (p->cmd1 != NULL)
		freetab(p->cmd1);
	if (p->cmd2 != NULL)
		freetab(p->cmd2);
	if (p->path1 != NULL)
		free(p->path1);
	if (p->path2 != NULL)
		free(p->path2);
	return (EXIT_SUCCESS);
}

void	errorreturn(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}
