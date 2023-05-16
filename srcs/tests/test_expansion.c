/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 14:18:21 by vegret            #+#    #+#             */
/*   Updated: 2023/05/16 17:18:54 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_expanded_size(t_env *env, char *str)
{
	int		size;
	int		j;
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

static char	*expand(t_env *env, char *str, char *dst) // Fix
{
	int		i;
	int		j;
	char	old;
	char	*content;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			j = i + 1;
			while (ft_isalnum(str[j]) || str[j] == '_')
				j++;
			old = str[j];
			str[j] = '\0';
			content = get_val(env, str + i + 1);
			str[j] = old;
			if (content)
			{
				ft_strlcpy(dst, content, ft_strlen(content) + 1);
				dst += ft_strlen(content);
			}
			i += j;
			continue ;
		}
		*dst++ = str[i++];
	}
	return (NULL);
}

char	*make_expansion(t_env *env, char *str)
{
	char	*expanded;
	int		size;

	size = get_expanded_size(env, str);
	expanded = malloc(size + 1 * sizeof(char));
	if (!expanded)
		return (NULL);
	expand(env, str, expanded);
	expanded[size - 1] = '\0';
	return (expanded);
}
