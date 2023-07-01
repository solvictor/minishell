/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:46:30 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/01 08:11:17 by nlegrand         ###   ########.fr       */
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
		ft_dprintf(STDERR_FILENO, "bash: pwd: %s\n", strerror(errno)); // NON, pas bash, minishell ou msh
		return (1);
	}
	if (ft_dprintf(STDOUT_FILENO, "%s\n", cwd) < 0)
	{
		ft_dprintf(STDERR_FILENO, "bash: pwd: write error: %s\n", // pareil ici
			strerror(errno));
		free(cwd);
		return (1);
	}
	free(cwd);
	return (0);
}
