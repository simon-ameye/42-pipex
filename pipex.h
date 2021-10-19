/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 14:52:26 by sameye            #+#    #+#             */
/*   Updated: 2021/10/19 12:41:09 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	**cmd1;
	char	**cmd2;
	char	*fnct1;
	char	*fnct2;
	char	*path1;
	char	*path2;
	char	*file1;
	char	*file2;
	int		pipefd[2];
}			t_pipex;

char	*findpath(char *fnct, char **envp);
int		process1(t_pipex p, char **envp);
int		process2(t_pipex p, char **envp);
int		freepipex(t_pipex *p);
int		fillpipex(t_pipex *p, char **av, char **envp);
int		printerror(char *str, int err);
void	freetab(char **tab);
char	*printnotfound(char *error, char *fnct);

#endif