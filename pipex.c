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
	//ft_putstr_fd("just before fill\n", 2);
	fillpipex(p, str, envp);
	if (p->path != NULL)
	{
		//ft_putstr_fd("just before execve\n", 2);
		if (execve(p->path, p->cmd, envp) == -1)
		{
			//ft_putstr_fd("execve = -1\n", 2);
			freepipex(p);
			//ft_putstr_fd("just before exit\n", 2);
			exit(perrorstring(str));
		}
		//ft_putstr_fd("just after execve\n", 2);
	}
	freepipex(p);
	exit(EXIT_FAILURE);
}

char	*threatcmd(char *cmd, char **envp)
{
	if (cmd != NULL)
	{
		if (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~')
		{
			if (access(cmd, F_OK) != 0)
			{
				ft_putstr_fd("no such file or directory: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd("\n", 2);
				return (NULL);
			}
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

void	createforks(t_pipex *p, char **av, char **envp)
{
	int		i;
	pid_t	pid;
	int		pipefd[2];
	int		tmpfd;

	tmpfd = p->infile;
	i = 0;
	while (i <= p->nbfunct - 1)
	{
		if (pipe(pipefd) == -1)
			exit(1);
		pid = fork();
		//printf("BEGIN tmpfd %i, i %i, nbfunct %i, pipefd[0] %i, PID %i\n", tmpfd, i, p->nbfunct, pipefd[0], pid);
		//printf("lets process %s\n", av[2 + i]);
		if (pid < 0)
			exit(1);
		else if (pid == 0)
		{
			//printf("processing %s\n", av[2 + i]);
			dup2(tmpfd, STDIN_FILENO);
			if (i == p->nbfunct - 1)
			{
				dup2(p->oufile, STDOUT_FILENO);
				if (p->oufile == -1)
					exit(EXIT_FAILURE);
			}
			else
				dup2(pipefd[1], STDOUT_FILENO);
			if  (tmpfd == -1)
				exit(EXIT_FAILURE);
			process(p, av[2 + i], envp);
		}
		else
		{
			//ft_putstr_fd("waiting son\n", 2);
			waitpid(pid, NULL, 0);
			//ft_putstr_fd("son just finished\n", 2);
			close(pipefd[1]);
			close(tmpfd);
			//dup2(pipefd[0], tmpfd);
			tmpfd = pipefd[0];
			//close(pipefd[0]);
		}
		//printf("END tmpfd %i, i %i, nbfunct %i, pipefd[0] %i, PID %i\n", tmpfd, i, p->nbfunct, pipefd[0], pid);
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
