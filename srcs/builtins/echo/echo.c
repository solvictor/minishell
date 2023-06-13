/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 23:05:14 by vegret            #+#    #+#             */
/*   Updated: 2023/06/13 15:48:31 by vegret           ###   ########.fr       */
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

int	builtin_echo(t_msh *msh, char **args)
{
	int		i;
	bool	newline;

	(void) msh;
	if (!args || !args[0])
		return (printf("\n"), 0);
	i = 1;
	while (is_flag_n(args[i]))
		i++;
	newline = i == 1;
	while (args[i])
	{
		printf("%s", args[i++]);
		if (args[i] != NULL)
			printf(" ");
	}
	if (newline)
		printf("\n");
	return (0);
}
