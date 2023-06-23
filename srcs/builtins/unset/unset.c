/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 19:05:35 by vegret            #+#    #+#             */
/*   Updated: 2023/06/23 17:11:00 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset(t_msh *msh, t_env *target)
{
	if (!msh->env || !target)
		return (1);
	if (target == msh->env)
	{
		msh->env = msh->env->next;
		if (msh->env)
			msh->env->prev = NULL;
	}
	else
	{
		target->prev->next = target->next;
		target->next->prev = target->prev;
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
		ret = unset(msh, get_env(msh->env, args[i], -1));
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}
