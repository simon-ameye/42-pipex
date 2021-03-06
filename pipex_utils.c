/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:10:11 by sameye            #+#    #+#             */
/*   Updated: 2021/11/08 17:40:28 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_findpaths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5) != NULL)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

char	*findpath(char *fnct, char **envp)
{
	int		i;
	char	**paths;
	char	*testpath1;
	char	*testpath2;

	paths = ft_findpaths(envp);
	if (paths)
	{
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
	}
	return (printstrings("command not found: ", fnct, "\n"));
}

char	*threatcmd(char *cmd, char **envp)
{
	if (cmd != NULL)
	{
		if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
		{
			if (access(cmd, F_OK) != 0)
				return (printstrings("No such file or directory: ", cmd, "\n"));
			return (ft_strjoin(cmd, ""));
		}
		else
			return (findpath(cmd, envp));
	}
	return (NULL);
}

void	fillpipex(t_pipex *p, char *str, char **envp)
{
	if (str != NULL)
	{
		if (str[0] != '\0')
		{
			p->cmd = ft_split(str, ' ');
			if (p->cmd != NULL)
				p->path = threatcmd((p->cmd)[0], envp);
		}
	}
}
