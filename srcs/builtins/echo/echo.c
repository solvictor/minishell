/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 23:05:14 by vegret            #+#    #+#             */
/*   Updated: 2023/07/03 13:51:02 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_flag_n(char *s)
{
	if (s == NULL || *s++ != '-')
		return (false);
	if (*s == '\0')
		return (false);
	while (*s)
		if (*s++ != 'n')
			return (false);
	return (true);
}

static int	print_args(char **args, bool newline, int i)
{
	while (args[i])
	{
		if (ft_dprintf(STDOUT_FILENO, "%s", args[i++]) < 0)
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

int	builtin_echo(t_msh *msh, char **args) // infinite loop on error
{
	int	i;
	int	ret;

	(void) msh;
	if (!args || !args[0])
		return (write(STDOUT_FILENO, "\n", 1) != 1);
	i = 1;
	while (is_flag_n(args[i]))
		i++;
	ret = print_args(args, i == 1, i);
	if (ret == 1)
		ft_dprintf(STDERR_FILENO, "bash: echo: write error: %s\n",
			strerror(errno));
	return (ret);
}
