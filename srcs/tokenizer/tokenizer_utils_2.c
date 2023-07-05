/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:17:20 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 17:01:47 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Puts a metachar token at the top of the token list
// Returns 0 on success, -1 otherwise
int	push_metachar_token(t_tokenlist **tokens, const char *input, int *i)
{
	if (token_add_front(tokens, NULL) == NULL)
		return (-1);
	(*tokens)->type = get_metachar_tokentype(input, i);
	return (0);
}

// Puts a string token at the top of the token list
// Returns 0 on success, -1 otherwise
int	push_str_token(t_tokenlist **tokens, const char *input, int *i)
{
	if (token_add_front(tokens, NULL) == NULL)
		return (-1);
	if (input[*i] == '"')
		(*tokens)->type = DOUBLE_QUOTED_STR;
	else if (input[*i] == '\'')
		(*tokens)->type = SINGLE_QUOTED_STR;
	else
		(*tokens)->type = UNQUOTED_STR;
	(*tokens)->data = get_str_token(input, i);
	if ((*tokens)->data == NULL)
		return (-1);
	if (input[*i] && !is_metachar(input[*i]) && !is_whitespace(input[*i]))
		(*tokens)->data_opt = 1;
	else
		(*tokens)->data_opt = 0;
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
		return (NULL);
	else if (ret == -2)
		return (ft_dprintf(STDERR_FILENO,
				"minishell: syntax error: unmatch quotation\n"), NULL);
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
