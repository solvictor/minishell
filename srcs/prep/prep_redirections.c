/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:05:50 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 11:37:40 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_pipes(t_cmdline *cmdline)
{
	int	i;
	int	fildes[2];

	i = 0;
	while (i < cmdline->cmds_n)
	{
		if (pipe(fildes) == -1)
			return (printf("Failed to open pipe #%d\n", i), -1);
		if (cmdline->fds[i * 2] != -1)
			close(fildes[0]);
		else
			cmdline->fds[i * 2] = fildes[0];
		if (cmdline->fds[i * 2 + 1] != -1)
			close(fildes[1]);
		else
			cmdline->fds[i * 2 + 1] = fildes[1];
		++i;
	}
	return (0);
}

int	do_redirections(t_cmdline *cmdline)
{
	//t_tokenlist	*cur;
	//int			i;

	//i = 0;
	//while (i < cmdline->cmds_n)
	//{
	//	cur = cmdline->cmds[i].start_token;
	//	while (cur && cur->type < PIPE)
	//	{
	//		if (is_redir_token(cur) && do_redir(cmdline, cur, i) == -1)
	//			return (printf("failed to do redirection\n"), -1);
	//		cur = cur->next;
	//	}
	//	++i;
	//}
	if (cmdline->cmds_n > 1 && open_pipes(cmdline) == -1)
		return (printf("failed to open pipes\n"), -1);
	return (0);
}

//int	do_redir_heredoc(t_cmdline *cmdline, t_tokenlist *token, int cmd_i)
//{
//	(void)cmdline;
//	(void)token;
//	printf("do_redir_heredoc filler function, probably gonna need it's own file for all the functions\n");
//	return (0);
//}
//
//int	do_redir(t_cmdline *cmdline, t_tokenlist *token)
//{
//	if (token->type == HEREDOC)
//		return (do_redir_heredoc(cmdline, token));
//	if (token->type == INPUTFILE)
//	{
//	}
//	return (0);
//}

