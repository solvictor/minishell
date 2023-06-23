/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 21:28:23 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/22 23:56:58 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns 0 if there are no syntax errors in the tokens
// Returns -1 otherwise
int	check_syntax_errors(t_tokenlist *tokens)
{
	t_tokenlist *cur;

	if (tokens == NULL)
		return (printf("NULL tokens, not a syntax error\n"), -1);
	cur = tokens;
	while (cur)
	{
		if (is_redir_token(cur) && !is_str_token(cur->next))
			return (printf("redir syntax error, print context...\n"), -1); // print something
		if (cur->type == PIPE && (cur == tokens || cur->next == NULL
				|| (cur->next && cur->next->type == PIPE))) // remove `&&` and all other bonus tokens and treat them as normal text
			return (printf("double pipes or pipe at beginning, print context...\n"), -1);
		if (cur->type == UNKNOWN) // didn't check if it's even possible for UNKNOWN token to appear here
			return (printf("unknown token, idk\n"), -1);
		cur = cur->next;
	}
	return (0);
}
// Removes the redirection tokens gives their property to the next string type
// token
void	clean_redir_tokens(t_tokenlist **tokens)
{
	t_tokenlist *cur;
	t_tokenlist **prev_next_ptr;

	prev_next_ptr = tokens;
	cur = *tokens;
	while (cur)
	{
		if (!is_redir_token(cur))
		{
			prev_next_ptr = &cur->next;
			cur = cur->next;
		}
		else
		{
			cur->next->type = cur->type;
			*prev_next_ptr = cur->next;
			prev_next_ptr = &cur->next->next;
			free(cur);
			cur = *prev_next_ptr;
		}
	}
}

