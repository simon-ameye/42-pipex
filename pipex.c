/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 13:06:04 by sameye            #+#    #+#             */
/*   Updated: 2021/09/23 16:41:32 by sameye           ###   ########.fr       */
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
			return (testpath2);
		i++;
	}
	return (NULL);
}

void	execcmd(char *cmd, char **envp)
{
	char *path;
	int	err;
	char **splitcmd;

	splitcmd = ft_split(cmd, ' ');
	path = findpath(splitcmd[0], envp);
	err = execve(path, splitcmd, envp);
	if (err == -1)
	{
		printf("cmd %s fail\n", splitcmd[0]);
		printf("path was : %s\n", path);
		exiterror();
	}
	free(path);
}

/*run a command with file as input and put result in pipe*/
void process1(char *fname, char *cmd, int *fd, char **envp)
{
	int file;

	file = open(fname, O_RDONLY, 0777);
	if (file == -1)
		exiterror();
	dup2(file, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	execcmd(cmd, envp);
}

/*run a command with pipe as input and put result in file*/
void process2(char *fname, char *cmd, int *fd, char **envp)
{
	int file;

	file = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (file == -1)
		exiterror();
	dup2(file, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	execcmd(cmd, envp);
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

int	main(int ac, char **av, char **envp)
{
	int	fd[2];
	int pid;

	(void) av;
	if (ac != 5)
		exiterror();
	if (pipe(fd) == -1)
		exiterror();
	pid = fork();
	if (pid == -1)
		exiterror();
	if (pid == 0)
		process1(av[1], av[2], fd, envp);
	waitpid(pid, NULL, 0);
	process2(av[4], av[3], fd, envp);
}