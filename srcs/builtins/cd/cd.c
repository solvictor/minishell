/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/09 19:50:23 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// !!! Work only on NULL terminating arrays
static int	arr_size(char **arr)
{
	int	size;

	if (!arr)
		return (-1);
	size = 0;
	while (arr[size])
		size++;
	return (size);
}

int	builtin_cd(t_msh *msh, char **args)
{
	const int	size = arr_size(args);
	char		*path;

	if (size > 2)
	{
		printf("bash: cd: too many arguments\n"); // Wrong message?
		return (-1);
	}
	// TODO See if we must handle '~' paths
	path = get_val(msh->env, "HOME");
	if (args[1])
		path = args[1];
	if (!path)
	{
		printf("No path to go\n");
		return (-1);
	}
	if (path[0] && chdir(path) != 0)
	{
		printf("%s\n", strerror(errno));
		return (-1);
	}
	return (0);
}
