/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 19:05:35 by vegret            #+#    #+#             */
/*   Updated: 2023/07/06 16:32:19 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset(t_msh *msh, t_env *target)
{
	if (!msh->env || !target)
		return ;
	if (target == msh->env)
	{
		msh->env = msh->env->next;
		if (msh->env)
			msh->env->prev = NULL;
	}
	else
	{
		target->prev->next = target->next;
		if (target->next)
			target->next->prev = target->prev;
	}
	free(target->var);
	free(target);
}

int	builtin_unset(t_msh *msh, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset(msh, get_env(msh->env, args[i], -1));
		i++;
	}
	return (0);
}
