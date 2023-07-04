/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:33:32 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/04 13:03:07 by nlegrand         ###   ########.fr       */
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
	else if (token->type == MERGED_STR)
		printf("MERGED_STR\n");
	else if (token->type == HEREDOC)
		printf("HEREDOC\n");
	else if (token->type == I_FILE)
		printf("I_FILE\n");
	else if (token->type == O_FILE_TRUNC)
		printf("O_FILE_TRUNC\n");
	else if (token->type == O_FILE_APPEND)
		printf("O_FILE_APPEND\n");
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
		if (curr->data_opt == 1)
			printf(" --->");
		printf("\n");
		display_token_type(curr);
		printf("\n");
		curr = curr->next;
		++i;
	}
}

void	display_cmdline(t_cmdline *cmdline)
{
	int	i;
	t_tokenlist *cur;
	int	j;

	printf("number of commands -> %d\n", cmdline->cmds_n);
	if (cmdline->cmds == NULL)
		return ((void)printf("pas encore malloc ta maman\n"));
	i = 0;
	while (i < cmdline->cmds_n)
	{
		printf("COMMAND #%d\n", i);
		if (cmdline->cmds[i].path)
			printf("path -> %s\n", cmdline->cmds[i].path);
		else if (cmdline->cmds[i].builtin)
			printf("builtin command!!\n");
		else if (cmdline->cmds[i].empty)
			printf("empty command...\n");
		else
			printf("command not found *0*\n");
		printf("tokens: ");
		cur = cmdline->cmds[i].start_token;
		while (cur && cur->type < PIPE)
		{
			if (is_str_token(cur))
				printf("%s ", cur->data);
			else if (cur->type == HEREDOC)
				printf("<< %s ", cur->data);
			else if (cur->type == I_FILE)
				printf("< %s ", cur->data);
			else if (cur->type == O_FILE_TRUNC)
				printf("> %s ", cur->data);
			else if (cur->type == O_FILE_APPEND)
				printf(">> %s ", cur->data);
			else
				printf("WAGNAGNAGNAALSKDLKFJAAOIEPW\n");
			cur = cur->next;
		}
		printf("\nargs: {");
		j = 0;
		while (cmdline->cmds[i].args && cmdline->cmds[i].args[j])
			printf("'%s', ", cmdline->cmds[i].args[j++]);
		printf("NULL}\n\n");
		++i;
	}
}

//int	do_redirs(t_cmdline *cmdline)
//{
//	t_tokenlist *cur;
//	int			i;
//	int			(*do_redir)(t_cmdline *, t_cmd *);
//	int			ret;
//
//	i = 0;
//	while (i < cmdline->cmds_n)
//	{
//		cur = cmdline->cmds[i].start_token;
//		while (cur && cur->type < PIPE)
//		{
//			if (is_redir_token(cur))
//			{
//				do_redir = get_redir_funptr(cur);
//				ret = do_redir(cmdline, &cmdline->cmds[i]);
//				if (ret == -1)
//					handle error
//			}
//		}
//		++i;
//	}
//	return (0);
//}
