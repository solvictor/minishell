/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:38:05 by vegret            #+#    #+#             */
/*   Updated: 2023/07/03 13:24:51 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(char *str)
{
	if (!*str || *str == '=' || ft_isdigit(*str))
		return (false);
	while (*str && *str != '=')
	{
		if (*str != '_' && !ft_isalnum(*str))
			return (false);
		str++;
	}
	return (true);
}

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
	msh->env = new;
	return (0);
}

int	builtin_export(t_msh *msh, char **args)
{
	int		ret;
	char	*equal;
	t_env	*new;

	ret = 0;
	while (*++args)
	{
		if (!is_valid_identifier(*args))
		{
			ft_dprintf(STDERR_FILENO,
				"bash: export: `%s': not a valid identifier\n", *args);
			ret = 1;
			continue ;
		}
		equal = ft_strchr(*args, '=');
		if (equal)
			new = get_env(msh->env, *args, equal - *args);
		else
			new = get_env(msh->env, *args, -1);
		if ((!new || equal) && assign_var(msh, new, *args)) // Malloc fail return value?
			return (-1);
	}
	return (ret);
}
