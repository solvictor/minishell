/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 20:14:11 by vegret            #+#    #+#             */
/*   Updated: 2023/07/11 02:17:57 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str, int *dst)
{
	unsigned long long	num;
	int					sign;

	sign = 1;
	num = 0;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		num = num * 10 + *str - '0';
		if ((num > LLONG_MAX && sign == 1)
			|| (num - 1 > LLONG_MAX && sign == -1))
			return (0);
		str++;
	}
	*dst = (sign * num) & 0xFF;
	return (1);
}

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
