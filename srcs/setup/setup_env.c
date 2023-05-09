/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:34:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/09 22:29:38 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_env(t_msh *msh, char **envp)
{
	int		i;
	t_env	**curr;

	(void) msh;
	i = 0;
	curr = &(msh->env);
	while (envp[i])
	{
		*curr = env_new(envp[i]);
		if (*curr == NULL)
			return (destroy_env_list(&msh->env), -1);
		curr = &(*curr)->next;
		++i;
	}
	return (0);
}
