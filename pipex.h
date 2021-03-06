/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 14:52:26 by sameye            #+#    #+#             */
/*   Updated: 2021/11/08 18:43:52 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <stdio.h>

typedef struct s_pipex
{
	char	**cmd;
	char	*path;
	int		infile;
	int		oufile;
	int		nbfunct;
	int		pipefd[2];
	int		code;
}			t_pipex;

void	process(t_pipex *p, char *str, char **envp);
void	child2(t_pipex *p, char *str, char **envp);
void	child1(t_pipex *p, char *str, char **envp);
void	createforks(t_pipex *p, char **av, char **envp);
int		main(int ac, char **av, char **envp);
char	**ft_findpaths(char **envp);
char	*findpath(char *fnct, char **envp);
char	*threatcmd(char *cmd, char **envp);
void	fillpipex(t_pipex *p, char *str, char **envp);
void	freetab(char **tab);
void	initpipex(t_pipex *p);
void	freepipex(t_pipex *p);
void	ft_putstr_fd(char *str, int fd);
int		perrorstring(char *str);
char	*printstrings(char *str1, char *str2, char *str3);
int		ft_count(char const *s, char c);
char	**ft_create_words(int count, char const *s, char c, char **res);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *str);
char	*ft_strnstr(const char *haystack,
			const char *needle, size_t len);
char	*ft_substr(char const *s, unsigned int start, size_t len);

#endif