/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 23:05:14 by vegret            #+#    #+#             */
/*   Updated: 2023/06/13 16:43:07 by vegret           ###   ########.fr       */
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
		if (printf("%s", args[i++]) < 0)
			return (EXIT_FAILURE);
		if (args[i] != NULL)
		{
			if (printf(" ") < 0)
				return (EXIT_FAILURE);
		}
	}
	if (newline && printf("\n") < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	builtin_echo(t_msh *msh, char **args)
{
	int	i;
	int	ret;

	(void) msh;
	if (!args || !args[0])
		return (printf("\n"), 0);
	i = 1;
	while (is_flag_n(args[i]))
		i++;
	ret = print_args(args, i == 1, i);
	if (ret == EXIT_FAILURE)
		perror("bash: echo: printf failed\n");
	return (ret);
}
