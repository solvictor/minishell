/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/08 15:30:41 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_env(t_env *env, char *cwd)
{
	t_env	*pwd;
	t_env	*oldpwd;

	pwd = get_env(env, "PWD", 3);
	oldpwd = get_env(env, "OLDPWD", 6);
	if (oldpwd)
	{
		free(oldpwd->var);
		if (pwd)
			oldpwd->var = ft_strjoin("OLDPWD=", pwd->var + 4);
		else
			oldpwd->var = ft_strdup("\0");
		if (!oldpwd->var)
			return (1);
	}
	if (pwd)
	{
		free(pwd->var);
		pwd->var = ft_strjoin("PWD=", cwd);
		free(cwd);
		if (!pwd->var)
			return (1);
	}
	return (0);
}

static char	*get_path(t_env *env, char *arg, bool go_oldpwd)
{
	const bool	go_home = arg == NULL;
	char		*var;

	if (go_home || go_oldpwd)
	{
		var = "HOME";
		if (go_oldpwd)
			var = "OLDPWD";
		arg = get_env_val(env, var);
		if (!arg)
		{
			ft_dprintf(STDERR_FILENO, "minishell: cd: %s not set\n", var);
			return (NULL);
		}
	}
	if (*arg == '\0')
		return (".");
	return (arg);
}

// TODO Test env modif
int	builtin_cd(t_msh *msh, char **args)
{
	char	*path;
	bool	go_oldpwd;

	if (args[1] && args[2])
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (1);
	}
	go_oldpwd = args[1] && ft_strncmp(args[1], "-\0", 2) == 0;
	path = get_path(msh->env, args[1], go_oldpwd);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd %s: %s\n", path,
			strerror(errno));
		return (1);
	}
	if (update_env(msh->env, getcwd(NULL, 0)))
		return (1);
	if (go_oldpwd)
		return (builtin_pwd(msh, args));
	return (0);
}
