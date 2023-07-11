/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 19:05:35 by vegret            #+#    #+#             */
/*   Updated: 2023/07/11 13:46:56 by vegret           ###   ########.fr       */
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
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]) || ft_strchr(args[i], '='))
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: unset: `%s': not a valid identifier\n", args[i]);
			ret = 1;
			i++;
			continue ;
		}
		unset(msh, get_env(msh->env, args[i], -1));
		i++;
	}
	return (ret);
}
