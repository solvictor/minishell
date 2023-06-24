/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 04:16:08 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/24 09:59:36 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Uses the PATH variable from the env to crate a str array of executable
// locations for pathfinding
char	**get_paths(t_env *env)
{
	char	**paths;
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (ft_strncmp(curr->var, "PATH=", 5) == 0)
			break ;
		curr = curr->next;
	}
	if (curr == NULL)
		return (NULL);
	paths = ft_split(curr->var + 5, ':'); // trouver une methode pour differencier PAS de PATH et une erreur de malloc
	return (paths);
}
