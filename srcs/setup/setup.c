/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 06:25:47 by nlegrand          #+#    #+#             */
/*   Updated: 2023/04/27 14:29:51 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	msh_init_vars(t_msh *msh)
{
	msh->env = NULL;
	msh->input = NULL;
	msh->tokens = NULL; // won't have to be in here later
	msh->ret_prev = 0;
	//msh->cmd.path = NULL; // remove later
	//msh->cmd.args = NULL; // remove later
}

//static int	set_signal_handlers(t_msh *msh) // probably dont need SA_RESTART so that it actually interrupts the function (NOT SURE AT ALL HOW THAT WORKS)
//{
//
//}

int	msh_setup(t_msh *msh, int ac, char **envp)
{
	if (ac != 1)
		return (printf(MSH_ERROR ME_AC), -1);
	msh_init_vars(msh);
	if (make_env(msh, envp) != 0)
		return (printf(MSH_ERROR ME_ENV), -1);
	//set_signal_handlers(msh);
	return (0);
}
