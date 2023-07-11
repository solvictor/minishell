/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:39:59 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/11 01:34:55 by nlegrand         ###   ########.fr       */
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
	tmp->prev = NULL;
	return (tmp);
}

// Destroy entire list of environment variables
void	destroy_env_list(t_env **env)
{
	t_env	*next;

	while (*env)
	{
		next = (*env)->next;
		free((*env)->var);
		free(*env);
		*env = next;
	}
}

// Converts the environment to a string array
// Return a pointer to the array or NULL otherwise
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

// Returns a pointer to the environment variable or NULL if the variable
// doesn't exist
t_env	*get_env(t_env *env, char *key, int len_key)
{
	if (!env || !key)
		return (NULL);
	if (len_key == -1)
		len_key = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(key, env->var, len_key) == 0
			&& (env->var[len_key] == '=' || env->var[len_key] == '\0'))
			return (env);
		env = env->next;
	}
	return (NULL);
}

// Returns the value of an environment variable without allocating or NULL if
// the variable doesn't exist
char	*get_env_val(t_env *env, char *key)
{
	int	len_key;

	if (!env || !key)
		return (NULL);
	if (key[0] != '_' && !ft_isalpha(key[0]))
		return (NULL);
	len_key = 0;
	while (key[len_key] == '_' || ft_isalnum(key[len_key]))
		++len_key;
	while (env)
	{
		if (ft_strncmp(key, env->var, len_key) == 0 && env->var[len_key] == '=')
			return (env->var + len_key + 1);
		env = env->next;
	}
	return (NULL);
}
