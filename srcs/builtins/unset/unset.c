/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 19:05:35 by vegret            #+#    #+#             */
/*   Updated: 2023/05/11 19:40:14 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*get_prec_env(t_env *env, char *key)
{
	int	len_key;

	if (!env || !key)
		return (NULL);
	len_key = ft_strlen(key);
	while (env && env->next)
	{
		if (ft_strncmp(key, env->next->var, len_key) == 0 // Maybe deleting wrong vars
			&& env->next->var[len_key] == '=')
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	unset(t_env *env, char *var)
{
	t_env	*prec;
	t_env	*target;

	// Handle if its first
	prec = get_prec_env(env, var);
	if (prec)
	{
		target = prec->next;
		prec->next = target->next;
		free(target->var);
		free(target);
	}
}

int	builtin_unset(t_msh *msh, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset(msh->env, args[i]);
		i++;
	}
	return (0);
}
