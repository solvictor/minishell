/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 06:39:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/24 07:16:48 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Calculates the size of the future expanded string after replacing the dollar
// variable being replaced by its content
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

// Makes new expanded string for token
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

// Expands a single token's data
int	expand_token(t_env *env, t_tokenlist *token)
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

// Loops through the token list and expands the proper tokens
int	do_dollar_expansions(t_msh *msh, t_tokenlist *tokens)
{
	t_tokenlist	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->type == UNQUOTED_STR || cur->type == DOUBLE_QUOTED_STR)
			if (expand_token(msh->env, cur) == -1)
				return (-1);
		cur = cur->next;
	}
	return (0);
}
