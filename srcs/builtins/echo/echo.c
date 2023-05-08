/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 23:05:14 by vegret            #+#    #+#             */
/*   Updated: 2023/05/08 23:30:23 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(t_msh *msh, char **args)
{
	int	i;
	int	newline;

	(void) msh;
	if (!args || !args[0])
		return (printf("\n"), 0);
	newline = 1;
	if (args[1] && ft_strncmp("-n", args[1], 3) == 0)
		newline = 0;
	i = 1 + !newline;
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
