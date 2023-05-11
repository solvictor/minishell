/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 20:14:11 by vegret            #+#    #+#             */
/*   Updated: 2023/05/11 14:46:17 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_msh *msh, char **args)
{
	unsigned char	status;

	status = EXIT_SUCCESS;
	if (args[1])
		status = (unsigned char) ft_atoi(args[1]);
	msh->exit = 1;
	msh->ret = status;
	return (0);
}
