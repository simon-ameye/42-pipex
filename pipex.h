/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 14:52:26 by sameye            #+#    #+#             */
/*   Updated: 2021/11/03 18:48:52 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>

typedef struct s_pipex
{
	char	**cmd[2];
	char	*path[2];
	int		inpt[2];
	int		oupt[2];
	int		pipefd[2];
	int		pid[2];
}			t_pipex;

char	*findpath(char *fnct, char **envp);
void	fillpipex(t_pipex *p, char *str, char **envp, int index);
int		main(int ac, char **av, char **envp);
void	freetab(char **tab);
void	initpipex(t_pipex *p);
void	freepipex(t_pipex *p);
void	ft_putstr_fd(char *str, int fd);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
char	*ft_strnstr(const char *haystack,
			const char *needle, size_t len);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		printerror(char *str1, char *str2, int err);
void	errorfree(t_pipex *p);
void	process(t_pipex *p, char **envp, int index);
void	createforks(t_pipex *p, char **av, char **envp);

#endif