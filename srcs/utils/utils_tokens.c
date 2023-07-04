/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:10:34 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/04 19:24:40 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

// Allocates a token struct and adds it to the beginning of the tokenlist
// Returns a pointer to the newly created token or NULL in case of error
t_tokenlist	*token_add_front(t_tokenlist **begin, char *data)
{
	t_tokenlist	*tmp;

	tmp = malloc(sizeof(t_tokenlist));
	if (tmp == NULL)
		return (NULL);
	tmp->data = data;
	tmp->type = UNKNOWN;
	tmp->data_opt = -1;
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}

// Frees the entire tokenlist structs along with it's data
void	destroy_tokenlist(t_tokenlist **begin)
{
	t_tokenlist	*tmp;

	if (begin == NULL || *begin == NULL)
		return ;
	while (*begin)
	{
		tmp = *begin;
		*begin = (*begin)->next;
		free(tmp->data);
		free(tmp);
	}
	*begin = NULL;
}

// Returns 1 if token is of REDIR type, 0 otherwise or if token is NULL
int	is_redir_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type >= HEREDOC && token->type <= O_FILE_APPEND);
}

// Returns 1 if token is of STR type, 0 otherwise or if token is NULL
int	is_str_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type <= MERGED_STR);
}

// Removes the redirection tokens gives their property to the next string type // token
void	clean_redir_tokens(t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	t_tokenlist	*tmp;

	cur = tokens;
	while (cur)
	{
		if (is_redir_token(cur))
		{
			cur->data = cur->next->data;
			tmp = cur->next;
			cur->next = tmp->next;
			free(tmp);
		}
		cur = cur->next;
	}
}
