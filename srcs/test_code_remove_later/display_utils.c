/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_display_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:33:32 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/23 02:07:40 by nlegrand         ###   ########.fr       */
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

void	display_pipeline(t_pipeline *pip)
{
	int	i;
	t_tokenlist *cur;

	printf("number of commands -> %d\n", pip->cmds_n);
	if (pip->cmds == NULL)
		return ((void)printf("pas encore malloc ta maman\n"));
	i = 0;
	while (i < pip->cmds_n)
	{
		printf("COMMAND #%d\n", i);
		cur = pip->cmds[i].start_token;
		while (cur && cur->type < PIPE)
		{
			if (is_str_token(cur))
				printf("%s ", cur->data);
			else if (cur->type == HEREDOC)
				printf("<< %s ", cur->data);
			else if (cur->type == INPUTFILE)
				printf("< %s ", cur->data);
			else if (cur->type == OUTPUTFILE_TRUNC)
				printf("> %s ", cur->data);
			else if (cur->type == OUTPUTFILE_APPEND)
				printf(">> %s ", cur->data);
			else
				printf("WAGNAGNAGNAALSKDLKFJAAOIEPW\n");
			cur = cur->next;
		}
		printf("\n");
		++i;
	}
}