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
			exit(printperror(""));
		}
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
				printerror("no such file or directory: ", cmd, EXIT_FAILURE);
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
		printf("tmpfd %i, i %i, nbfunct %i, pipefd[0] %i\n", tmpfd, i, p->nbfunct, pipefd[0]);
		printf("lets process %s\n", av[2 + i]);
		if (pid < 0)
			exit(1);
		else if (pid == 0)
		{
			if  (tmpfd == -1)
				exit(EXIT_FAILURE);
			printf("processing %s\n", av[2 + i]);
			dup2(tmpfd, STDIN_FILENO);
			if (i == p->nbfunct - 1)
				dup2(p->oufile, STDOUT_FILENO);
			else
				dup2(pipefd[1], STDOUT_FILENO);
			process(p, av[2 + i], envp);
		}
		else
		{
			waitpid(-1, NULL, 0);
			close(pipefd[1]);
			close(tmpfd);
			dup2(pipefd[0], tmpfd);
			close(pipefd[0]);
		}
		i++;
	}
	close(p->oufile);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	p;

	initpipex(&p);
	if (ac != 5)
		return (printerror("Error: Wrong arguments", "", EXIT_FAILURE));
	p.infile = open(av[1], O_RDONLY, 0777);
	if (p.infile == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(av[1], 2);
		ft_putstr_fd("\n", 2);
	}
	p.oufile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (p.oufile == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(av[4], 2);
		ft_putstr_fd("\n", 2);
	}
	p.nbfunct = ac - 3;
	createforks(&p, av, envp);
}
