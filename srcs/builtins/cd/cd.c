/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/07 20:02:14 by vegret           ###   ########.fr       */
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
	char		*new_path;

	(void) msh;
	if (size > 2)
	{
		printf("bash: cd: too many arguments\n"); // Wrong message?
		return (-1);
	}
	new_path = "~"; // No path = go home
	if (args[1])
		new_path = args[1];
	// just testing the chdir function, not at all final behavior
	if (chdir(new_path) != 0)
	{
		printf("%s\n", strerror(errno));
		return (-1);
	}
	return (0);
}
