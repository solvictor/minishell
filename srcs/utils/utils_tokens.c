/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:10:34 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/22 23:55:51 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Allocates a token struct and adds it to the beginning of the tokenlist
// Returns a pointer to the newly created token or NULL in case of error
t_tokenlist	*token_add_front(t_tokenlist **begin, char *data)
{
	t_tokenlist	*tmp;

	tmp = malloc(sizeof(t_tokenlist));
	if (tmp == NULL)
		return (NULL);
	tmp->data = data;
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}

// Frees the entire tokenlist structs along with it's data
void	destroy_tokenlist(t_tokenlist **begin)
{
	t_tokenlist	*tmp;

	while (*begin)
	{
		tmp = *begin;
		*begin = (*begin)->next;
		free(tmp->data);
		free(tmp);
	}
}

int	is_redir_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type >= HEREDOC && token->type <= OUTPUTFILE_APPEND);
}

int	is_str_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type <= MERGED_STR);
}
