/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:39:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/04/29 17:44:46 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Creates a new environment variable node
t_env	*env_new(char *var)
{
	t_env	*tmp;

	tmp = malloc(sizeof(t_env));
	if (tmp == NULL)
		return (NULL);
	tmp->var = ft_strdup(var);
	if (tmp->var == NULL)
		return (free(tmp), NULL);
	tmp->next = NULL;
	return (tmp);
}

// Destroy entire list of environment variables
void	destroy_env_list(t_env **env)
{
	t_env *next;

	while (*env)
	{
		next = (*env)->next;
		free((*env)->var);
		free(*env);
		*env = next;
	}
}

char	**env_to_arr(t_env *env)
{
	char	**envarr;
	t_env	*curr;
	int		i;

	curr = env;
	i = 0;
	while (curr)
	{
		++i;
		curr = curr->next;
	}
	envarr = malloc(sizeof(char *) * (i + 1));
	if (envarr == NULL)
		return (NULL);
	curr = env;
	i = 0;
	while (curr)
	{
		envarr[i++] = curr->var;
		curr = curr->next;
	}
	envarr[i] = NULL;
	return (envarr);
}
