/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 21:28:23 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 15:20:47 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_token(t_tokenlist *token)
{
	if (token == NULL)
		ft_dprintf(STDERR_FILENO, "newline");
	else if (is_str_token(token))
		ft_dprintf(STDERR_FILENO, "%s", token->data);
	else if (token->type == HEREDOC)
		ft_dprintf(STDERR_FILENO, "<<", token->data);
	else if (token->type == I_FILE)
		ft_dprintf(STDERR_FILENO, "<", token->data);
	else if (token->type == O_FILE_TRUNC)
		ft_dprintf(STDERR_FILENO, ">", token->data);
	else if (token->type == O_FILE_APPEND)
		ft_dprintf(STDERR_FILENO, ">>", token->data);
	else if (token->type == PIPE)
		ft_dprintf(STDERR_FILENO, "|", token->data);
	else
		ft_dprintf(STDERR_FILENO, "unkown", token->data);
}


static void	print_syntax_error(t_tokenlist *token)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error near unexpected token `");
	print_token(token->next);
	ft_dprintf(STDERR_FILENO, "'\n");
}

// Returns 0 if there are no syntax errors in the tokens
// Returns -1 otherwise
int	check_syntax_errors(t_tokenlist *tokens)
{
	t_tokenlist	*cur;

	cur = tokens;
	while (cur)
	{
		if (is_redir_token(cur) && !is_str_token(cur->next))
			break ;
		if (cur->type == PIPE && (cur == tokens || cur->next == NULL
				|| (cur->next && cur->next->type == PIPE))) // remove `&&` and all other bonus tokens and treat them as normal text
			break ;
		if (cur->type == UNKNOWN) // didn't check if it's even possible for UNKNOWN token to appear here
			break ;
		cur = cur->next;
	}
	if (cur != NULL)
		return (print_syntax_error(cur), -1);
	return (0);
}
