/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 20:10:11 by sameye            #+#    #+#             */
/*   Updated: 2021/09/24 17:44:24 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	printerror(char *str, int err)
{
	ft_putstr_fd("Error: ", 1);
	ft_putstr_fd(str, 1);
	ft_putstr_fd("\n", 1);
	return (err);
}

int	exiterror(char *str, int err)
{
	ft_putstr_fd("Error: ", 1);
	ft_putstr_fd(str, 1);
	ft_putstr_fd("\n", 1);
	exit(err);
}

void freetab(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}