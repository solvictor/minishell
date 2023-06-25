/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_merge.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 05:43:54 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/24 07:22:56 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

static int	merge_token(t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	int			len;
	char		*tmp;

	len = 0;
	cur = tokens;
	while (cur && cur->merge_next)
	{
		len += ft_strlen(cur->data);
		cur = cur->next;
	}
	len += ft_strlen(cur->data);
	tmp = malloc(sizeof(char) * (len + 1));
	if (tmp == NULL)
		return (-1);
	tmp[0] = '\0';
	cur = tokens;
	while (cur && cur->merge_next)
	{
		ft_strlcat(tmp, cur->data, len + 1);
		cur = cur->next;
	}
	ft_strlcat(tmp, cur->data, len + 1);
	free(tokens->data);
	tokens->data = tmp;
	return (0);
}

// Removes the tokens that have been merged to the first token from the list
static void	discard_merge_rest(t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	t_tokenlist	*tmp;

	cur = tokens->next;
	while (cur && cur->merge_next)
	{
		tmp = cur->next;
		free(cur->data);
		free(cur);
		cur = tmp;
	}
	tokens->next = cur->next;
	free(cur->data);
	free(cur);
}

int	merge_str_tokens(t_tokenlist *tokens)
{
	t_tokenlist *cur;

	cur = tokens;
	while (cur)
	{
		if (cur->merge_next)
		{
			if (merge_token(cur) == -1)
				return (-1);
			discard_merge_rest(cur);
			cur->type = MERGED_STR;
		}
		cur = cur->next;
	}
	return (0);
}