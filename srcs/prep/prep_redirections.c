/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:05:50 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/04 10:50:44 by nlegrand         ###   ########.fr       */
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

// Identifies the type of redirection and does it
// Return 0 on success, -1 otherwise
static int	do_redir(t_cmd *cmd, t_tokenlist *token)
{
	if (token->type == INPUTFILE)
	{
		close_valid_fds(cmd->redirs, 1);
		cmd->redirs[0] = open(token->data, O_RDONLY, 0644);
		if (cmd->redirs[0] == -1)
			return (ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
						token->data, strerror(errno)), -1);
	}
	else if (token->type == OUTPUTFILE_TRUNC || token->type == OUTPUTFILE_APPEND)
	{
		close_valid_fds(cmd->redirs, 1);
		if (token->type == OUTPUTFILE_TRUNC)
			cmd->redirs[1] = open(token->data, O_CREAT | O_TRUNC | O_WRONLY,
				0644);
		else
			cmd->redirs[1] = open(token->data, O_CREAT | O_APPEND | O_WRONLY,
				0644);
		if (cmd->redirs[1] == -1) // not handled properly, for < inputfile.txt cat checklist (for example), maybe reimplement has_input_redir
			return (ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
					token->data, strerror(errno)), -1);
	}
	return (0);
}

int	do_redirections(t_cmdline *cmdline)
{
	t_tokenlist	*cur;
	int			i;

	i = 0;
	while (i < cmdline->cmds_n)
	{
		cur = cmdline->cmds[i].start_token;
		while (cur && cur->type < PIPE)
		{
			if (is_redir_token(cur) && do_redir(&cmdline->cmds[i], cur) == -1)
				break ;
			cur = cur->next;
		}
		++i;
	}
	if (cmdline->cmds_n > 1 && open_pipes(cmdline) == -1)
		return (printf("failed to open pipes\n"), -1);
	return (0);
}
