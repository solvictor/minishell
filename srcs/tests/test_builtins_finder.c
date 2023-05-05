/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins_finder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 18:43:41 by vegret            #+#    #+#             */
/*   Updated: 2023/05/05 19:22:00 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define NB_BUILTINS 2

// !!! Work only on NULL terminating arrays
static char	*last_elem(char **arr)
{
	int	size;

	if (!arr)
		return (NULL);
	size = 0;
	while (arr[size])
		size++;
	if (!size)
		return (NULL);
	return (arr[size - 1]);
}

t_builtin	get_builtin(t_cmd *cmd)
{
	const char		*last = last_elem(cmd->args);
	const char		*names[] = {"cd", "pwd"};
	const t_builtin	funcs[] = {builtin_cd, builtin_pwd};
	int				index;
	int				len;

	if (!last)
		return (NULL);
	len = ft_strlen(last);
	index = 0;
	while (index < NB_BUILTINS)
	{
		if (ft_strncmp(last, names[index], len) == 0)
			return (funcs[index]);
		index++;
	}
	return (NULL);
}
