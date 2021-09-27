#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include "libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	char **cmd1;
	char **cmd2;
	char *fnct1;
	char *fnct2;
	char *path1;
	char *path2;
	char *file1;
	char *file2;
	int pipefd[2];
}		t_pipex;

int	printerror(char *str, int err);
void freetab(char **tab);
int	exiterror(char *str, int err);
int perrorfail(void);

#endif