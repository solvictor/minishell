/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/10 21:57:12 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_oldpwd(t_msh *msh)
{
	int		ret;
	char	*var;
	char	*pwd;

	pwd = get_env_val(msh->env, "PWD");
	if (pwd && *pwd)
		var = ft_strjoin("OLDPWD=", pwd);
	else
		var = "OLDPWD=";
	if (!var)
		return (1);
	ret = builtin_export(msh, (char *[]){"export", var, NULL});
	if (pwd && *pwd)
		free(var);
	return (ret);
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

int	builtin_cd(t_msh *msh, char **args)
{
	char	*path;
	bool	go_oldpwd;

	if (args[1] && args[2])
		return (ft_dprintf(STDERR_FILENO,
					"minishell: cd: too many arguments\n"), 1);
	go_oldpwd = args[1] && ft_strncmp(args[1], "-\0", 2) == 0;
	path = get_path(msh->env, args[1], go_oldpwd);
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (ft_dprintf(STDERR_FILENO, "minishell: cd %s: %s\n", path,
				strerror(errno)), 1);
	path = get_env_val(msh->env, "OLDPWD");
	if (set_oldpwd(msh) || set_pwd(msh))
		return (1);
	if (go_oldpwd)
		if (ft_printf("%s\n", path) < 0)
			return (ft_dprintf(STDERR_FILENO,
					"minishell: cd: write error: %s\n", strerror(errno)), 1);
	return (0);
}
