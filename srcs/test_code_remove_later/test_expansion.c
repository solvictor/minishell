/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:18:21 by vegret            #+#    #+#             */
/*   Updated: 2023/06/23 22:52:49 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_expanded_size(t_env *env, char *str)
{
	int		j;
	int		size;
	char	old;
	char	*content;

	size = 0;
	while (*str)
	{
		if (*str++ == '$')
		{
			j = 0;
			while (ft_isalnum(str[j]) || str[j] == '_')
				j++;
			old = str[j];
			str[j] = '\0';
			content = get_env_val(env, str);
			str[j] = old;
			if (content)
				size += ft_strlen(content);
			str += j;
			continue ;
		}
		size++;
	}
	return (size);
}

static void	expand(t_env *env, char *str, char *dst)
{
	int		j;
	char	old;
	char	*content;

	while (*str)
	{
		if (*str == '$')
		{
			j = 1;
			while (ft_isalnum(str[j]) || str[j] == '_')
				j++;
			old = str[j];
			str[j] = '\0';
			content = get_env_val(env, str + 1);
			str[j] = old;
			if (content)
				while (*content)
					*dst++ = *content++;
			str += j;
			continue ;
		}
		*dst++ = *str++;
	}
}

//char	*make_expansion(t_env *env, char *str)
int	make_expansion(t_env *env, t_tokenlist *token)
{
	int		size;
	char	*expanded;

	if (token->data && ft_strchr(token->data, '$') == NULL)
		return (0);
	size = get_expanded_size(env, token->data) + 1;
	expanded = malloc(sizeof(char) * size);
	if (!expanded)
		return (-1);
	expand(env, token->data, expanded);
	expanded[size - 1] = '\0';
	free(token->data);
	token->data = expanded;
	return (0);
}

