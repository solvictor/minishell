/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 16:38:05 by vegret            #+#    #+#             */
/*   Updated: 2023/05/11 17:56:58 by vegret           ###   ########.fr       */
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
	int		i;
	char	*equal;
	t_env	*new;

	i = 0;
	while (args[++i])
	{
		equal = ft_strchr(args[i], '=');
		if (equal) // No '=' ?
		{
			equal[0] = '\0';
			new = get_env(msh->env, args[i]);
			equal[0] = '=';
			if (assign_var(msh, new, args[i]))
				return (-1);
		}
	}
	return (0);
}
