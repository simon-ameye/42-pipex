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

char	*findpath(char *fnct, char **envp)
{
	int		i;
	char	**paths;
	char	*testpath1;
	char	*testpath2;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		testpath1 = ft_strjoin(paths[i], "/");
		testpath2 = ft_strjoin(testpath1, fnct);
		free(testpath1);
		if (access(testpath2, F_OK) == 0)
		{
			freetab(paths);
			return (testpath2);
		}
		free(testpath2);
		i++;
	}
	freetab(paths);
	printerror("command not found: ", fnct, EXIT_FAILURE);
	return (NULL);
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

void	initpipex(t_pipex *p)
{
	p->cmd[0] = NULL;
	p->cmd[1] = NULL;
	p->path[0] = NULL;
	p->path[1] = NULL;
}

void	freepipex(t_pipex *p)
{
	if (p->cmd[0] != NULL)
		freetab(p->cmd[0]);
	if (p->path[0] != NULL)
		free(p->path[0]);
	if (p->cmd[1] != NULL)
		freetab(p->cmd[1]);
	if (p->path[1] != NULL)
		free(p->path[1]);
}
