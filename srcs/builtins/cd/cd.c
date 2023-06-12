/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/09 19:54:04 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_msh *msh, char **args)
{
	char	*path;

	if (args[1] && args[2])
	{
		ft_dprintf(STDERR_FILENO, "bash: cd: too many arguments\n");
		return (1);
	}
	path = get_env_val(msh->env, "HOME");
	if (!path)
	{
		ft_dprintf(STDERR_FILENO, "bash: cd: HOME not set\n");
		return (1);
	}
	if (args[1])
		path = args[1];
	if (path[0] && chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "bash: cd %s: %s\n", path, strerror(errno));
		return (1);
	}
	return (0);
}
