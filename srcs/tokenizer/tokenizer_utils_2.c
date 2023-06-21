/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:17:20 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/21 20:44:17 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Puts a metachar token at the top of the token list
// Returns 0 on success, -1 otherwise
int	push_metachar_token(t_tokenlist **tokens, const char *input, int *i)
{
	if (token_add_front(tokens, NULL) == NULL)
		return (printf("malloc error token_add_front\n"), -1);
	(*tokens)->type = get_metachar_tokentype(input, i);
	(*tokens)->concat_next = 0;
	return (0);
}

// Puts a string token at the top of the token list
// Returns 0 on success, -1 otherwise
int	push_str_token(t_tokenlist **tokens, const char *input, int *i)
{
	if (token_add_front(tokens, NULL) == NULL)
		return (printf("malloc error token_add_front\n"), -1);
	if (input[*i] == '"')
		(*tokens)->type = DOUBLE_QUOTED_STR;
	else if (input[*i] == '\'')
		(*tokens)->type = SINGLE_QUOTED_STR;
	else
		(*tokens)->type = UNQUOTED_STR;
	(*tokens)->data = get_str_token(input, i);
	if ((*tokens)->data == NULL)
		return (printf("malloc error get_str_token\n"), -1);
	if (input[*i] && !is_metachar(input[*i]) && !is_whitespace(input[*i]))
		(*tokens)->concat_next = 1;
	else
		(*tokens)->concat_next = 0;
	return (0);
}

// Returns a string extracted from the input
// Works for quoted and unquoted strings
// Returns NULL on error
char	*get_str_token(const char *input, int *i)
{
	int		ret;
	char	*tmp;

	if (input[*i] == '"' || input[*i] == '\'')
		ret = get_str_quoted(input + *i, &tmp);
	else
		ret = get_str_unquoted(input + *i, &tmp);
	if (ret == -1)
		return (printf("malloc error in get_str_token\n"), NULL);
	else if (ret == -2)
		return (printf("Unmatch quotation\n"), NULL);
	*i += ret;
	return (tmp);
}

// Reverses the token list at the end of the tokenization because tokens are
// pushed to the top of the list instead of at the end
void	reverse_tokens(t_tokenlist **begin)
{
	t_tokenlist	*curr;
	t_tokenlist	*prev;
	t_tokenlist	*next;

	curr = *begin;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*begin = prev;
}
