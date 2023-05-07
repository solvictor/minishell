/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins_finder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:43:41 by vegret            #+#    #+#             */
/*   Updated: 2023/05/07 22:03:28 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define NB_BUILTINS 3

t_builtin	get_builtin(t_cmd *cmd)
{
	const char		*names[] = {"cd", "pwd", "exit"};
	const t_builtin	funcs[] = {builtin_cd, builtin_pwd, builtin_exit};
	int				index;
	int				len;

	if (!cmd->args || !cmd->args[0])
		return (NULL);
	len = ft_strlen(cmd->args[0]);
	index = 0;
	while (index < NB_BUILTINS)
	{
		if (ft_strncmp(cmd->args[0], names[index], len) == 0)
			return (funcs[index]);
		index++;
	}
	return (NULL);
}
