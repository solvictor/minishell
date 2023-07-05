/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_redirections_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:05:50 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 16:16:04 by nlegrand         ###   ########.fr       */
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
			return (-1);
		if (cmdline->pipes[i * 2] != -1)
			close(fildes[0]);
		else
			cmdline->pipes[i * 2] = fildes[0];
		if (cmdline->pipes[i * 2 + 1] != -1)
			close(fildes[1]);
		else
			cmdline->pipes[i * 2 + 1] = fildes[1];
		++i;
	}
	return (0);
}

// Performs all redirections for all commands in cmdline
// If a redirection fails for a command all subsequent redirections for that
// command are cancelled
// Returns 0 on success, -1 otherwise
int	do_redirections(t_cmdline *cmdline)
{
	t_tokenlist	*cur;
	int			i;
	int			ret;

	i = 0;
	while (i < cmdline->cmds_n)
	{
		cur = cmdline->cmds[i].start_token;
		while (cur && cur->type < PIPE)
		{
			if (is_redir_token(cur) && cur->type <= I_FILE)
				ret = do_redir_input(&cmdline->cmds[i], cur);
			else if (is_redir_token(cur))
				ret = do_redir_output(&cmdline->cmds[i], cur);
			if (is_redir_token(cur) && ret == -1)
				break ;
			cur = cur->next;
		}
		++i;
	}
	if (cmdline->cmds_n > 1 && open_pipes(cmdline) == -1)
		return (-1);
	return (0);
}
