/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:05:50 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 22:08:42 by nlegrand         ###   ########.fr       */
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

// Does heredoc
static int	do_redir_heredoc(t_cmdline *cmdline, t_cmd *cmd, t_tokenlist *token)
{
	(void)cmdline;
	(void)cmd;
	(void)token;
	printf("do_redir_heredoc filler function, probably gonna need it's own file for all the functions\n");
	return (0);
}

// Identifies the type of redirection and does it
// Return 0 on success, -1 otherwise
static int	do_redir(t_cmdline *cmdline, t_cmd *cmd, t_tokenlist *token)
{
	if (token->type == HEREDOC)
		return (do_redir_heredoc(cmdline, cmd, token));
	else if (token->type == INPUTFILE)
	{
		cmd->io_redir[0] = open(token->data, O_RDONLY, 0644);
		if (cmd->io_redir[0] == -1) // not handled properly, for < inputfile.txt cat checklist (for example), maybe reimplement has_input_redir
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", token->data,
				strerror(errno));
	}
	else if (token->type == OUTPUTFILE_TRUNC || token->type == OUTPUTFILE_APPEND)
	{
		if (token->type == OUTPUTFILE_TRUNC)
			cmd->io_redir[1] = open(token->data, O_CREAT | O_TRUNC | O_WRONLY,
				0644);
		else
			cmd->io_redir[1] = open(token->data, O_CREAT | O_APPEND | O_WRONLY,
				0644);
		if (cmd->io_redir[1] == -1) // not handled properly, for < inputfile.txt cat checklist (for example), maybe reimplement has_input_redir
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", token->data,
				strerror(errno));
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
			if (is_redir_token(cur) && do_redir(cmdline,
				&cmdline->cmds[i], cur) == -1)
				return (printf("failed to do redirection\n"), -1);
			cur = cur->next;
		}
		++i;
	}
	if (cmdline->cmds_n > 1 && open_pipes(cmdline) == -1)
		return (printf("failed to open pipes\n"), -1);
	return (0);
}
