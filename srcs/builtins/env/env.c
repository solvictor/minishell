/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:19:58 by vegret            #+#    #+#             */
/*   Updated: 2023/07/03 13:28:03 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_msh *msh, char **args)
{
	t_env	*cur;

	(void) args;
	cur = msh->env;
	while (cur)
	{
		if (ft_strchr(cur->var, '='))
		{
			if (ft_dprintf(STDOUT_FILENO, "%s\n", cur->var) < 0)
			{
				ft_dprintf(STDERR_FILENO, "bash: pwd: write error: %s\n",
					strerror(errno));
				return (125);
			}
		}
		cur = cur->next;
	}
	return (0);
}
