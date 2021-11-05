/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:06:04 by sameye            #+#    #+#             */
/*   Updated: 2021/11/03 18:40:26 by sameye           ###   ########.fr       */
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
			exit(perrorstring(str));
		}
	}
	freepipex(p);
	exit(EXIT_FAILURE);
}

void	child(t_pipex *p, char *str, int i, char **envp)
{
	dup2(p->tmpfd, STDIN_FILENO);
	if (i == p->nbfunct - 1)
	{
		dup2(p->oufile, STDOUT_FILENO);
		if (p->oufile == -1)
			exit(EXIT_FAILURE);
	}
	else
		dup2(p->pipefd[1], STDOUT_FILENO);
	if (p->tmpfd == -1)
		exit(EXIT_FAILURE);
	process(p, str, envp);
}

void	createforks(t_pipex *p, char **av, char **envp)
{
	int		i;
	pid_t	pid;

	p->tmpfd = p->infile;
	i = 0;
	while (i <= p->nbfunct - 1)
	{
		if (pipe(p->pipefd) == -1)
			exit(1);
		pid = fork();
		if (pid < 0)
			exit(1);
		else if (pid == 0)
			child(p, av[2 + i], i, envp);
		else
		{
			waitpid(pid, NULL, 0);
			close(p->pipefd[1]);
			close(p->tmpfd);
			p->tmpfd = p->pipefd[0];
		}
		i++;
	}
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
	p.infile = open(av[1], O_RDONLY, 0777);
	if (p.infile == -1)
		perrorstring(av[1]);
	p.nbfunct = ac - 3;
	p.oufile = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (p.oufile == -1)
		perrorstring(av[ac - 1]);
	createforks(&p, av, envp);
}
