/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:38:05 by vegret            #+#    #+#             */
/*   Updated: 2023/05/31 17:38:54 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_identifier(char *str)
{
	while (*str)
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
			return (-1);
		return (0);
	}
	new = env_new(var);
	if (!new)
		return (-1);
	new->next = msh->env;
	msh->env = new;
	return (0);
}

int	builtin_export(t_msh *msh, char **args) // See where to add new variables
{
	int		ret;
	char	*equal;
	t_env	*new;

	ret = EXIT_SUCCESS;
	while (*++args)
	{
		equal = ft_strchr(*args, '=');
		if (equal) // No '=' ?
		{
			equal[0] = '\0';
			if (!is_valid_identifier(*args))
			{
				ft_dprintf(STDERR_FILENO,
					"bash: export: `%s': not a valid identifier\n", *args);
				ret = EXIT_FAILURE;
				continue ;
			}
			new = get_env(msh->env, *args);
			equal[0] = '=';
			if (assign_var(msh, new, *args))
				return (-1);
		}
	}
	return (ret);
}
