/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 23:05:14 by vegret            #+#    #+#             */
/*   Updated: 2023/07/11 14:44:51 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_flag_n(char *s)
{
	if (s == NULL || *s++ != '-')
		return (0);
	if (*s == '\0')
		return (0);
	while (*s)
		if (*s++ != 'n')
			return (0);
	return (1);
}

static int	print_args(char **args, int newline, int i)
{
	while (args[i])
	{
		if (ft_printf("%s", args[i++]) < 0)
			return (1);
		if (args[i] != NULL)
		{
			if (write(STDOUT_FILENO, " ", 1) != 1)
				return (1);
		}
	}
	if (newline)
		return (write(STDOUT_FILENO, "\n", 1) != 1);
	return (0);
}

int	builtin_echo(t_msh *msh, char **args)
{
	int	i;
	int	ret;

	(void) msh;
	if (!args || !args[0])
	{
		ret = write(STDOUT_FILENO, "\n", 1) != 1;
		if (ret)
			ft_dprintf(STDERR_FILENO, "minishell: echo: write error: %s\n",
				strerror(errno));
		return (ret);
	}
	i = 1;
	while (is_flag_n(args[i]))
		i++;
	ret = print_args(args, i == 1, i);
	if (ret)
		ft_dprintf(STDERR_FILENO, "minishell: echo: write error: %s\n",
			strerror(errno));
	return (ret);
}
