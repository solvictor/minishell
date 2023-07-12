/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 20:14:11 by vegret            #+#    #+#             */
/*   Updated: 2023/07/12 18:30:29 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_msh *msh, char **args)
{
	int	ret;

	msh->exit = 1;
	if (isatty(STDIN_FILENO))
		ft_dprintf(STDERR_FILENO, "exit\n");
	if (!args[1])
		return (msh->ret);
	if (is_numeric(args[1], &ret) && !args[2])
		return (ret);
	if (!is_numeric(args[1], &msh->ret))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", args[1]);
		return (2);
	}
	return ((msh->exit = 0), ft_dprintf(STDERR_FILENO,
			"minishell: exit: too many arguments\n"), 1);
}
