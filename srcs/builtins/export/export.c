/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:38:05 by vegret            #+#    #+#             */
/*   Updated: 2023/07/11 13:37:16 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	assign_var(t_msh *msh, t_env *new, char *var)
{
	if (new)
	{
		free(new->var);
		new->var = ft_strdup(var);
		if (!new->var)
			return (1);
		return (0);
	}
	new = env_new(var);
	if (!new)
		return (1);
	new->next = msh->env;
	msh->env->prev = new;
	msh->env = new;
	return (0);
}

static int	print_env(t_env	*env)
{
	int		ret;
	char	*equal;

	while (env)
	{
		equal = ft_strchr(env->var, '=');
		if (equal)
		{
			*equal = '\0';
			ret = printf("export %s=\"%s\"\n", env->var, equal + 1);
			*equal = '=';
		}
		else
			ret = printf("export %s\n", env->var);
		if (ret < 0)
		{
			ft_dprintf(STDERR_FILENO, "minishell: export: write error: %s\n",
				strerror(errno));
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int	builtin_export(t_msh *msh, char **args)
{
	int		ret;
	char	*equal;
	t_env	*new;

	ret = 0;
	if (!args[1])
		return (print_env(msh->env));
	while (*++args)
	{
		if (!is_valid_identifier(*args))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: export: `%s': not a valid identifier\n", *args);
			ret = 1;
			continue ;
		}
		equal = ft_strchr(*args, '=');
		if (equal)
			new = get_env(msh->env, *args, equal - *args);
		else
			new = get_env(msh->env, *args, -1);
		if ((!new || equal) && assign_var(msh, new, *args))
			return (1);
	}
	return (ret);
}
