/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 20:14:11 by vegret            #+#    #+#             */
/*   Updated: 2023/05/15 18:12:42 by vegret           ###   ########.fr       */
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
	*dst = (unsigned char)(sign * num);
	return (1);
}

int	builtin_exit(t_msh *msh, char **args)
{
	ft_dprintf(2, "exit\n");
	msh->exit = 1;
	if (args[1])
	{
		if (args[2])
		{
			ft_dprintf(2, "bash: exit: too many arguments\n");
			msh->exit = 0;
			return (1);
		}
		if (!is_numeric(args[1], &msh->ret))
		{
			ft_dprintf(2,
				"bash: exit: %s: numeric argument required\n", args[1]);
			msh->ret = 2;
			return (2);
		}
	}
	return (0);
}
