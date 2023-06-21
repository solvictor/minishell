/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_remove_this_file.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:33:32 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/21 20:30:58 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_token_type(t_tokenlist *token)
{
	printf("token type -> ");
	if (token->type == UNQUOTED_STR)
		printf("UNQUOTED_STR\n");
	else if (token->type == DOUBLE_QUOTED_STR)
		printf("DOUBLE_QUOTED_STR\n");
	else if (token->type == SINGLE_QUOTED_STR)
		printf("SINGLE_QUOTED_STR\n");
	else if (token->type == HEREDOC)
		printf("HEREDOC\n");
	else if (token->type == INPUTFILE)
		printf("INPUTFILE\n");
	else if (token->type == OUTPUTFILE_TRUNC)
		printf("OUTPUTFILE_TRUNC\n");
	else if (token->type == OUTPUTFILE_APPEND)
		printf("OUTPUTFILE_APPEND\n");
	else if (token->type == PIPE)
		printf("PIPE\n");
	else
		printf("UNKNOWN\n");
}

void	display_tokens(t_tokenlist *begin)
{
	t_tokenlist	*curr;
	int			i;

	curr = begin;
	i = 0;
	while (curr)
	{
		printf("TOKEN #%d\n", i);
		printf("token data -> %s", curr->data);
		if (curr->concat_next)
			printf(" --->");
		printf("\n");
		display_token_type(curr);
		printf("\n");
		curr = curr->next;
		++i;
	}
}

void	test_tokenizer(t_msh *msh)
{
	int			ret;
	t_tokenlist	*tokens; // can be made into an ArrayList

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return ((void)printf(MSH_ERROR ME_TOKENIZE));
	display_tokens(tokens);
	destroy_tokenlist(&tokens);
}
