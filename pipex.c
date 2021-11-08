/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:06:04 by sameye            #+#    #+#             */
/*   Updated: 2021/11/08 17:37:28 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	process(t_pipex *p, char *str, char **envp)
{
	fillpipex(p, str, envp);
	if (p->path != NULL)
	{
		if (execve(p->path, p->cmd, envp) == -1)
		{
			freepipex(p);
			perrorstring(str);
			exit(127);
		}
	}
	freepipex(p);
	exit(127);
}

void	child2(t_pipex *p, char *str, char **envp)
{
	dup2(p->pipefd[0], STDIN_FILENO);
	dup2(p->oufile, STDOUT_FILENO);
	close(p->pipefd[1]);
	if (p->pipefd[0] == -1 || p->oufile == -1)
		exit(EXIT_FAILURE);
	process(p, str, envp);
	exit(127);
}

void	child1(t_pipex *p, char *str, char **envp)
{
	dup2(p->infile, STDIN_FILENO);
	dup2(p->pipefd[1], STDOUT_FILENO);
	close(p->pipefd[0]);
	if (p->infile == -1 || p->pipefd[1] == -1)
		exit(EXIT_FAILURE);
	close(p->infile);
	process(p, str, envp);
	exit(127);
}

void	createforks(t_pipex *p, char **av, char **envp)
{
	int	pid1;
	int	pid2;

	if (pipe(p->pipefd) == -1)
		exit(1);
	pid1 = fork();
	if (pid1 < 0)
		return (perror("Fork: "));
	else if (pid1 == 0)
		child1(p, av[2], envp);
	pid2 = fork();
	if (pid2 < 0)
		return (perror("Fork: "));
	else if (pid2 == 0)
		child2(p, av[3], envp);
	close(p->pipefd[0]);
	close(p->pipefd[1]);
	waitpid(pid1, &(p->code), 0);
	p->code = WEXITSTATUS(p->code);
	close(p->infile);
	waitpid(pid2, &(p->code), 0);
	p->code = WEXITSTATUS(p->code);
	close(p->oufile);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	p;

	initpipex(&p);
	if (ac < 5)
	{
		ft_putstr_fd("Error: Wrong arguments\n", 2);
		return (EXIT_FAILURE);
	}
	p.infile = open(av[1], O_RDONLY);
	if (p.infile == -1)
	{
		p.code = 1;
		perrorstring(av[1]);
	}
	p.nbfunct = ac - 3;
	p.oufile = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (p.oufile == -1)
		perrorstring(av[ac - 1]);
	createforks(&p, av, envp);
	return (p.code);
}
