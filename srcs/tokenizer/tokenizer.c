/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:52:25 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/23 00:02:16 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenize(const char *input, t_tokenlist **tokens)
{
	int			i;

	*tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			++i;
		if (input[i] && !is_metachar(input[i]))
		{
			if (push_str_token(tokens, input, &i) == -1)
				return (destroy_tokenlist(tokens), -1);
		}
		else if (input[i])
			if (push_metachar_token(tokens, input, &i) == -1)
				return (destroy_tokenlist(tokens), -1);
	}
	reverse_tokens(tokens);
	if (check_syntax_errors(*tokens) == -1)
		return (printf("Syntax error return\n"), destroy_tokenlist(tokens), -1);
	clean_redir_tokens(tokens);
	return (0);
}