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

void	initpipex(t_pipex *p)
{
	p->cmd = NULL;
	p->path = NULL;
}

void	freepipex(t_pipex *p)
{
	if (p->cmd != NULL)
		freetab(p->cmd);
	if (p->path != NULL)
		free(p->path);
}
