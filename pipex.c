/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:06:04 by sameye            #+#    #+#             */
/*   Updated: 2021/09/24 17:53:33 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*find PATH in envp and try to find fnct path inside*/
char	*findpath(char *fnct, char **envp)
{
	int i;
	char **paths;
	char *testpath1;
	char *testpath2;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	paths = ft_split(envp[i], ':');
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

/*run a command with file as input and put result in pipe*/
int process1(t_pipex p, char **envp)
{
	int file;

	file = open(p.file1, O_RDONLY, 0777);
	if (file == -1)
		return(printerror("Unable to open file for process1", EXIT_FAILURE));
	dup2(file, STDIN_FILENO);
	dup2(p.pipefd[1], STDOUT_FILENO);
	close(p.pipefd[0]);
	execve(p.path1, p.cmd1, envp);
	return (EXIT_SUCCESS);
}

/*run a command with pipe as input and put result in file*/
int process2(t_pipex p, char **envp)
{
	int file;

	file = open(p.file2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file == -1)
		return (printerror("Unable to open file for process2", EXIT_FAILURE));
	dup2(file, STDOUT_FILENO);
	dup2(p.pipefd[0], STDIN_FILENO);
	close(p.pipefd[1]);
	execve(p.path2, p.cmd2, envp);
	return (EXIT_SUCCESS);
}

void printenv(char **envp)
{
	int i;

	i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

int initpipex(t_pipex *p)
{
	p->cmd1 = NULL;
	p->cmd2 = NULL;
	p->path1 = NULL;
	p->path2 = NULL;
	return (EXIT_SUCCESS);
}

int freepipex(t_pipex *p)
{
	if (p->cmd1 != NULL)
		freetab(p->cmd1);
	if (p->cmd2 != NULL)
		freetab(p->cmd2);
	if (p->path1 != NULL)
		free(p->path1);
	if (p->path2 != NULL)
		free(p->path2);
	return (EXIT_SUCCESS);
}

int fillpipex(t_pipex *p, char **av, char **envp)
{
	p->cmd1 = ft_split(av[2], ' ');
	p->cmd2 = ft_split(av[3], ' ');
	p->fnct1 = p->cmd1[0];
	p->fnct2 = p->cmd2[0];
	p->file1 = av[1];
	p->file2 = av[4];
	p->path1 = findpath(p->fnct1, envp);
	if (p->path1 == NULL)
		return(printerror("Command not found.", 127));
	p->path2 = findpath(p->fnct2, envp);
	if (p->path2 == NULL)
		return(printerror("Command not found.", 127));
	return(EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	p;
	int child1;
	int child2;

	if (ac != 5)
		return(printerror("Wrong number of arguments", EXIT_FAILURE));
	if (pipe(p.pipefd) == -1)
		return(printerror("Pipe creation fail", EXIT_FAILURE));
	initpipex(&p);
	if (fillpipex(&p, av, envp) != EXIT_SUCCESS)
	{
		freepipex(&p);
		return(127);
	}
	child1 = fork();
	if (child1 == -1)
		return(printerror("Unable to create child1", EXIT_FAILURE));
	if (child1 == 0)
		process1(p, envp);
	child2 = fork();
	if (child2 == -1)
		return(printerror("Unable to create child2", EXIT_FAILURE));
	if (child2 == 0)
		process2(p, envp);
	close(p.pipefd[0]);
	close(p.pipefd[1]);
	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
}