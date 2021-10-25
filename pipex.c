/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:06:04 by sameye            #+#    #+#             */
/*   Updated: 2021/10/25 16:34:04 by sameye           ###   ########.fr       */
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
	return (NULL);
}

int	process1(t_pipex p, char **envp)
{
	int	file;

	file = open(p.file1, O_RDONLY, 0777);
	if (file == -1)
	{
		errorreturn();
		return (EXIT_FAILURE);
	}
	dup2(file, STDIN_FILENO);
	dup2(p.pipefd[1], STDOUT_FILENO);
	close(p.pipefd[0]);
	if (execve(p.path1, p.cmd1, envp) == -1)
		errorreturn();
	return (EXIT_SUCCESS);
}

int	process2(t_pipex p, char **envp)
{
	int	file;

	file = open(p.file2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file == -1)
	{
		errorreturn();
		return (EXIT_FAILURE);
	}
	dup2(file, STDOUT_FILENO);
	dup2(p.pipefd[0], STDIN_FILENO);
	close(p.pipefd[1]);
	if (execve(p.path2, p.cmd2, envp) == -1)
		errorreturn();
	return (EXIT_SUCCESS);
}

int	fillpipex(t_pipex *p, char **av, char **envp)
{
	p->cmd1 = ft_split(av[2], ' ');
	p->cmd2 = ft_split(av[3], ' ');
	p->fnct1 = p->cmd1[0];
	p->fnct2 = p->cmd2[0];
	p->file1 = av[1];
	p->file2 = av[4];
	p->path1 = findpath(p->fnct1, envp);
	p->path2 = findpath(p->fnct2, envp);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	p;
	int		child1;
	int		child2;

	if (ac != 5)
		return (printerror2("Error: Wrong arguments", "", EXIT_FAILURE));
	if (pipe(p.pipefd) == -1)
		errorreturn();
	fillpipex(&p, av, envp);
	child1 = fork();
	if (child1 == -1)
		errorreturn();
	if (child1 == 0)
		process1(p, envp);
	child2 = fork();
	if (child2 == -1)
		errorreturn();
	if (child2 == 0)
		process2(p, envp);
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
	freepipex(&p);
}
