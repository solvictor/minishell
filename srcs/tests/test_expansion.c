/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:18:21 by vegret            #+#    #+#             */
/*   Updated: 2023/05/30 13:56:29 by vegret           ###   ########.fr       */
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
			content = get_val(env, str);
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
			content = get_val(env, str + 1);
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

char	*make_expansion(t_env *env, char *str)
{
	int		size;
	char	*expanded;

	size = get_expanded_size(env, str) + 1;
	expanded = malloc(size * sizeof(char));
	if (!expanded)
		return (NULL);
	expand(env, str, expanded);
	expanded[size - 1] = '\0';
	return (expanded);
}
