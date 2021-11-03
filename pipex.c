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

void	process(t_pipex *p, char **envp, int i)
{
	if (p->inpt[i] == -1)
		errorfree(p);
	dup2(p->inpt[i], STDIN_FILENO);
	dup2(p->oupt[i], STDOUT_FILENO);
	close(p->pipefd[i]);
	if (p->path[i] != NULL)
	{
		if (execve(p->path[i], p->cmd[i], envp) == -1)
			errorfree(p);
	}
	freepipex(p);
	exit(EXIT_FAILURE);
}

char *threatcmd(char *cmd, char **envp)
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

void	fillpipex(t_pipex *p, char *str, char **envp, int index)
{
	if (str != NULL)
	{
		if (str[0] != '\0')
		{
			p->cmd[index] = ft_split(str, ' ');
			if (p->cmd[index] != NULL)
				p->path[index] = threatcmd((p->cmd[index])[0], envp);
		}
	}
}

void createforks(t_pipex *p, char **av, char **envp)
{
	int i;

	i = 0;
	while (i <= 1)
	{
		fillpipex(p, av[2 + i], envp, i);
		p->pid[i] = fork();
		if (p->pid[i] == -1)
			errorfree(p);
		if (p->pid[i] == 0)
			process(p, envp, i);
		i++;
	}
	i = 0;
	while (i <= 1)
	{
		waitpid(p->pid[i], NULL, 0);
		close(p->oupt[i]);
		close(p->inpt[i]);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	p;

	initpipex(&p);
	if (ac != 5)
		return (printerror("Error: Wrong arguments", "", EXIT_FAILURE));
	if (pipe(p.pipefd) == -1)
		errorfree(&p);
	p.inpt[0] = open(av[1], O_RDONLY, 0777);
	p.oupt[0] = p.pipefd[1];
	p.oupt[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	p.inpt[1] = p.pipefd[0];
	createforks(&p, av, envp);
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