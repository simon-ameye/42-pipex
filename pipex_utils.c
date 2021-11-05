/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:10:11 by sameye            #+#    #+#             */
/*   Updated: 2021/11/03 18:46:12 by sameye           ###   ########.fr       */
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
	i = -1;
	while (paths[++i])
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
	}
	freetab(paths);
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(fnct, 2);
	ft_putstr_fd("\n", 2);
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
