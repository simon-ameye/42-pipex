/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:06:04 by sameye            #+#    #+#             */
/*   Updated: 2021/11/02 17:54:29 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int		isabsolute(char *cmd)
{
	return (cmd[0] == '/' || cmd[0] == '.' || cmd[0] == '~');
}

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

void	process1(t_pipex *p, char **envp)
{
	if (p->fdfile1 == -1)
		errorfree(p);
	dup2(p->fdfile1, STDIN_FILENO);
	dup2(p->pipefd[1], STDOUT_FILENO);
	close(p->pipefd[0]);
	if (p->path1 != NULL)
	{
		if (execve(p->path1, p->cmd1, envp) == -1)
			errorfree(p);
	}
	freepipex(p);
	exit(EXIT_FAILURE);
}

void	process2(t_pipex *p, char **envp)
{
	if (p->fdfile2 == -1)
		errorfree(p);
	dup2(p->fdfile2, STDOUT_FILENO);
	dup2(p->pipefd[0], STDIN_FILENO);
	close(p->pipefd[1]);
	if (p->path2 == NULL)
	{
		freepipex(p);
		exit(EXIT_FAILURE);
	}
	if (execve(p->path2, p->cmd2, envp) == -1)
		errorfree(p);
}

char *threatcmd(char *cmd, char **envp)
{
	if (cmd != NULL)
	{
		if (isabsolute(cmd))
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

void	fillpipex(t_pipex *p, char **av, char **envp)
{

	if (av[2] != NULL)
	{
		if (av[2][0] != '\0')
		{
			p->cmd1 = ft_split(av[2], ' ');
			if (p->cmd1 != NULL)
				p->path1 = threatcmd((p->cmd1)[0], envp);
		}
	}
	if (av[3] != NULL)
	{
		if (av[3][0] != '\0')
		{
			p->cmd2 = ft_split(av[3], ' ');
			if (p->cmd2 != NULL)
			{
				p->path2 = threatcmd((p->cmd2)[0], envp);
			}
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	p;
	int		child1;
	int		child2;

	initpipex(&p);
	if (ac != 5)
		return (printerror("Error: Wrong arguments", "", EXIT_FAILURE));
	if (pipe(p.pipefd) == -1)
		errorfree(&p);
	p->fdfile1 = open(av[1], O_RDONLY, 0777);
	p->fdfile2 = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	fillpipex(&p, av, envp);
	child1 = fork();
	if (child1 == -1)
		errorfree(&p);
	if (child1 == 0)
		process1(&p, envp);
	child2 = fork();
	if (child2 == -1)
		errorfree(&p);
	if (child2 == 0)
		process2(&p, envp);
	waitpid(child1, NULL, 0);
	close(p.pipefd[1]);
	waitpid(child2, NULL, 0);
	close(p.pipefd[0]);
	close(p.fdfile1);
	close(p.fdfile2);
	freepipex(&p);
}

/*
2 lignes secu ajouter split en début
et free
leaks avec un argumebt null
chemin absolu
valgrind leaks lors du pas de droits chmod
commandes en relatif ex : ./
Pas executer avec un chemin faux !
=> écrire sois même les erreurs
relink : voir les photos
proteger les fd lors d'une erreur de lecture
*/