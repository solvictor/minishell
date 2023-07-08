/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:46:30 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/07 15:10:31 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_msh *msh, char **args)
{
	char	*cwd;

	(void) msh;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
			args[0], strerror(errno));
		return (1);
	}
	if (printf("%s\n", cwd) < 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: write error: %s\n",
			args[0], strerror(errno));
		free(cwd);
		return (1);
	}
	free(cwd);
	return (0);
}
