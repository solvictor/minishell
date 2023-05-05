/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:46:30 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/05 19:49:09 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_msh *msh, char **args)
{
	char	*cwd;

	(void) msh;
	(void) args;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		// Check if its the good way to handle
		printf("%s\n", strerror(errno));
		return (-1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
