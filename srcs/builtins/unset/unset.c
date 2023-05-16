/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 19:05:35 by vegret            #+#    #+#             */
/*   Updated: 2023/05/13 21:42:55 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset(t_msh *msh, char *var)
{
	t_env	*target;

	if (!msh->env || !var)
		return (1);
	target = get_env(msh->env, var);
	if (!target)
		return (1);
	if (target == msh->env)
	{
		msh->env = msh->env->next;
		msh->env->prec = NULL;
	}
	else
	{
		target->prec->next = target->next;
		target->next->prec = target->prec;
	}
	free(target->var);
	free(target);
	return (0);
}

int	builtin_unset(t_msh *msh, char **args)
{
	int	i;
	int	ret;

	i = 1;
	while (args[i])
	{
		ret = unset(msh, args[i]);
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}