/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:29:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/06 22:05:24 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_heredoc_tokens(t_tokenlist **tokens)
{
	t_tokenlist *curr;
	t_tokenlist *prev;

	prev = NULL;
	curr = *tokens;
	while (curr)
	{
		if (curr->type == L_ARROW_DOUBLE && (curr->next == NULL || curr->next->type > SINGLE_QUOTED_STR))
			return (-1);
		if (curr->type == L_ARROW_DOUBLE && curr->next->type <= SINGLE_QUOTED_STR)
		{
			// remove curr (optional) and modify next type to HEREDOC
			curr->next->type = HEREDOC;
			if (prev)
				prev->next = curr->next;
			else
				*tokens = curr->next;
			prev = curr->next;
			free(curr);
			curr = prev->next;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	return (0);
}

void	test_parsing(t_msh *msh)
{
	int			ret;
	t_tokenlist	*tokens;

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return ((void)printf(MSH_ERROR ME_TOKENIZE));

	display_tokens(tokens);
	if (set_heredoc_tokens(&tokens) == -1)
		return (printf("Wrong heredoc without delimiter probably\n"),
			destroy_tokenlist(&tokens));
	display_tokens(tokens);

	destroy_tokenlist(&tokens);
}
