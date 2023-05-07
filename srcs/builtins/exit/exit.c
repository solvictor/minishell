/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 20:14:11 by vegret            #+#    #+#             */
/*   Updated: 2023/05/07 23:46:55 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_msh *msh, char **args)
{
	unsigned char	status;

	status = EXIT_SUCCESS;
	if (args[1])
		status = (unsigned char) ft_atoi(args[1]);
	// TODO free all
	free(msh->input);
	msh_terminate(msh);
	exit(status);
	return (0);
}
