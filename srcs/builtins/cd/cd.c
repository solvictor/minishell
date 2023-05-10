/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/10 19:01:37 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_msh *msh, char **args)
{
	char	*path;

	if (args[1] && args[2])
	{
		printf("bash: cd: too many arguments\n"); // Wrong message?
		return (-1);
	}
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
