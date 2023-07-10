/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/10 23:47:43 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Gives OLDPWD the value of PWD
static int	set_oldpwd(t_msh *msh)
{
	int		ret;
	char	*var;
	char	*pwd;

	pwd = get_env_val(msh->env, "PWD");
	if (pwd == NULL)
		return (-1);
	if (pwd )
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

//static char	*get_path(t_env *env, char *arg, bool go_oldpwd)
//{
//	const bool	go_home = arg == NULL;
//	char		*var;
//
//	if (go_home || go_oldpwd)
//	{
//		var = "HOME";
//		if (go_oldpwd)
//			var = "OLDPWD";
//		arg = get_env_val(env, var);
//		if (!arg)
//		{
//			ft_dprintf(STDERR_FILENO, "minishell: cd: %s not set\n", var);
//			return (NULL);
//		}
//	}
//	if (*arg == '\0')
//		return (".");
//	return (arg);
//}
//
//int	builtin_cd(t_msh *msh, char **args)
//{
//	char	*path;
//	bool	go_oldpwd;
//
//	if (args[1] && args[2])
//		return (ft_dprintf(STDERR_FILENO,
//					"minishell: cd: too many arguments\n"), 1);
//	go_oldpwd = args[1] && ft_strncmp(args[1], "-\0", 2) == 0;
//	path = get_path(msh->env, args[1], go_oldpwd);
//	if (!path)
//		return (1);
//	if (chdir(path) != 0)
//		return (ft_dprintf(STDERR_FILENO, "minishell: cd %s: %s\n", path,
//				strerror(errno)), 1);
//	path = get_env_val(msh->env, "OLDPWD");
//	if (set_oldpwd(msh) || set_pwd(msh))
//		return (1);
//	if (go_oldpwd)
//		if (ft_printf("%s\n", path) < 0)
//			return (ft_dprintf(STDERR_FILENO,
//					"minishell: cd: write error: %s\n", strerror(errno)), 1);
//	return (0);
//}

static int	go_home(t_msh *msh)
{
	const char *home = get_env_val;

	if (home == NULL)
		return (ft_dprintf(STDERR_FILENO, "minishell: cd: HOME not set\n"), -1);
	if (chdir(home) != 0)
		return (ft_dprintf(STDERR_FILENO, "minishell: cd %s: %s\n", path,
				strerror(errno)), -1);
	return (0);
}

// Changes the direction of the process
int	builtin_cd(t_msh *msh, char **args)
{
	if (args[1] && args[2])
		return (ft_dprintf(STDERR_FILENO,
			"minishell: cd: too many arguments\n"), 1);
	if (args[1] == NULL && go_home(msh) == -1)
		return (1);
	//else if (ft_strncmp(args[1], "-\0", 2) == 0)
	//	// go to OLDPWD
	//else
	//	// do chdir and s
	//
	return (0);
}
