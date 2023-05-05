/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:37:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/05 18:15:01 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_msh *msh, char **args)
{
	// just testing the chdir function, not at all final behavior
	//if (chdir(path) != 0)
	//	printf("chdir failed.\n");
	(void) msh;
	(void) args;
	return (0);
}
