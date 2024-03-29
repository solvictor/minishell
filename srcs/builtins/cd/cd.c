/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/12 19:56:40 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	cd_write_error(void)
{
	ft_dprintf(STDERR_FILENO, "minishell: cd: write error: %s\n",
		strerror(errno));
}

// Uses an environment variable's value as path
// Returns 0 on success and -1 otherwise
static int	cd_env(t_msh *msh, char *var)
{
	const char	*dest = get_env_val(msh->env, var);

	if (dest == NULL)
		return (ft_dprintf(STDERR_FILENO,
				"minishell: cd: %s not set\n", var), -1);
	if (chdir(dest) != 0)
		return (ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", dest,
				strerror(errno)), -1);
	if (ft_strncmp(var, "OLDPWD", ft_strlen(var) + 1) == 0
		&& ft_printf("%s\n", dest) < 0)
		return (cd_write_error(), -1);
	return (set_pwds(msh));
}

static int	chdir_cdpath(t_msh *msh, char *arg)
{
	char	**paths;
	int		i;
	char	*tmp;

	paths = get_paths(msh->env, "CDPATH");
	if (paths == NULL)
		return (-1);
	i = 0;
	while (paths[i])
	{
		tmp = concat_path(paths[i], arg);
		if (tmp == NULL)
			return (clear_strarr(paths), -1);
		if (chdir(tmp) == 0)
		{
			free(tmp);
			tmp = getcwd(NULL, 0);
			if (tmp != NULL && ft_printf("%s\n", tmp) < 0)
				return (clear_strarr(paths), free(tmp), cd_write_error(), -1);
			return (clear_strarr(paths), free(tmp), 0);
		}
		free(tmp);
		++i;
	}
	return (clear_strarr(paths), -1);
}

static int	cd_normal(t_msh *msh, char *arg)
{
	if (chdir(arg) == 0)
		return (set_pwds(msh));
	if (chdir_cdpath(msh, arg) == 0)
		return (set_pwds(msh));
	ft_dprintf(STDERR_FILENO, "minishell: cd: %s: %s\n", arg, strerror(errno));
	return (-1);
}

// Changes the direction of the process
int	builtin_cd(t_msh *msh, char **args)
{
	if (args[1] && args[2])
		return (ft_dprintf(STDERR_FILENO,
				"minishell: cd: too many arguments\n"), 1);
	if (args[1] == NULL)
		return (cd_env(msh, "HOME") != 0);
	else if (ft_strncmp(args[1], "-\0", 2) == 0)
		return (cd_env(msh, "OLDPWD") != 0);
	else
		return (cd_normal(msh, args[1]) != 0);
	return (0);
}
