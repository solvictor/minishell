/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 06:39:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 18:50:16 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	copy_num(int n, char *dst)
{
	int		len;
	int		i;
	long	nn;

	if (n < 0)
		dst[0] = '-';
	nn = (long)n * (1 - (n < 0) * 2);
	len = ft_numlen(n);
	i = 0;
	while (nn > 9)
	{
		dst[len - i++ - 1] = '0' + nn % 10;
		nn /= 10;
	}
	dst[len - i++ - 1] = '0' + nn % 10;
	return (len);
}

// Calculates the lenght of the future expanded string after replacing the
// dollar variable being replaced by its content
static int	get_expanded_len(t_msh *msh, char *str)
{
	int		len;
	char	*val;

	len = 0;
	while (*str)
	{
		if (*str == '$' && str[1] == '?' && ++str && ++str)
			len += ft_numlen(msh->ret);
		else if (*str == '$' && (ft_isalpha(str[1]) || str[1] == '_') && ++str)
		{
			val = get_env_val(msh->env, str);
			if (val != NULL)
				len += ft_strlen(val);
			while (ft_isalnum(*str) || *str == '_')
				++str;
		}
		else
		{
			++str;
			++len;
		}
	}
	return (len);
}

// Makes new expanded string for token
static void	expand(t_msh *msh, char *str, char *dst)
{
	char	*val;

	while (*str)
	{
		if (*str == '$' && str[1] == '?' && ++str && ++str)
			dst += copy_num(msh->ret, dst);
		else if (*str == '$' && (ft_isalpha(str[1]) || str[1] == '_') && ++str)
		{
			val = get_env_val(msh->env, str);
			if (val)
				dst += ft_strlcpy(dst, val, ft_strlen(val) + 1);
			while (*str == '_' || ft_isalnum(*str))
				++str;
		}
		else
			*(dst++) = *(str++);
	}
}

// Expands a single string
int	expand_str(t_msh *msh, char **str)
{
	int		size;
	char	*expanded;

	if (*str && ft_strchr(*str, '$') == NULL)
		return (0);
	size = get_expanded_len(msh, *str) + 1;
	expanded = malloc(sizeof(char) * size);
	if (!expanded)
		return (-1);
	expand(msh, *str, expanded);
	expanded[size - 1] = '\0';
	free(*str);
	*str = expanded;
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
			if (expand_str(msh, &cur->data) == -1)
				return (-1);
		cur = cur->next;
	}
	return (0);
}
