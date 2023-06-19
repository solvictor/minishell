/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 06:25:47 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/17 00:05:07 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	msh_init_vars(t_msh *msh)
{
	msh->env = NULL;
	msh->input = NULL;
	//msh->tokens = NULL; // won't have to be in here later
	msh->ret = EXIT_SUCCESS;
	msh->exit = 0;
}

int	msh_setup(t_msh *msh, int ac, char **envp)
{
	if (ac != 1)
		return (printf(MSH_ERROR ME_AC), -1);
	msh_init_vars(msh);
	if (setup_signals() != 0)
		return (printf(MSH_ERROR ME_ENV), -1);
	if (make_env(msh, envp) != 0)
		return (printf(MSH_ERROR ME_ENV), -1);
	if (init_rng(&msh->rng) == -1)
		return (printf(MSH_ERROR ME_RNG), destroy_env_list(&msh->env), -1);
	return (0);
}
