/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 13:23:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 22:30:50 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Bool function returns 1 if input redirection is found in command tokens
// 0 otherwise
int	has_input_redir(t_cmd *cmd)
{
	t_tokenlist *cur;

	cur = cmd->start_token;
	while (cur && cur->type < PIPE)
	{
		if (cur->type == HEREDOC || cur->type == INPUTFILE)
			return (1);
		cur = cur->next;
	}
	return (0);
}

// Bool function returns 1 if output redirection is found in command tokens
// 0 otherwise
int	has_output_redir(t_cmd *cmd)
{
	t_tokenlist *cur;

	cur = cmd->start_token;
	while (cur && cur->type < PIPE)
	{
		if (cur->type == OUTPUTFILE_TRUNC || cur->type == OUTPUTFILE_APPEND)
			return (1);
		cur = cur->next;
	}
	return (0);
}

// Uses dup2 to redirect standard input and output properly for pipelines or io
// redirections
int	redirect_io(t_cmdline *cmdline, t_cmd *cmd)
{
	int	error;

	error = 0;
	if (cmd->io_redir[0] != -1)
		error += dup2(cmd->io_redir[0], STDIN_FILENO) == -1;
	else if (cmd->num != 0
		&& cmdline->cmds[cmd->num - 1].io_redir[1] == -1)
		error += dup2(cmdline->fds[cmd->num * 2 - 2], STDIN_FILENO) == -1;
	if (cmd->io_redir[1] != -1)
		error += dup2(cmd->io_redir[1], STDOUT_FILENO) == -1;
	else if (cmd->num != cmdline->cmds_n - 1)
		error += dup2(cmdline->fds[cmd->num * 2 + 1], STDOUT_FILENO) == -1;
	close_valid_fds(cmdline->fds, cmdline->cmds_n * 2);
	close_valid_fds(cmd->io_redir, 2);
	return ((error != 0) * -1);
}

// Redirects inputs and outputs for builtin commands and saves the old io in
// io_dup to reestablish it later
int	redirect_builtin_io(t_cmdline *cmdline, t_cmd *cmd, int io_dup[2])
{
	int	error;

	error = 0;
	if ((has_input_redir(cmd) && cmd->io_redir[0] != -1) || cmd->num != 0)
	{
		io_dup[0] = dup(STDIN_FILENO);
		if (io_dup[0] == -1)
			return (printf("failed to dup STDIN_FILENO\n"), -1);
		if (cmd->io_redir[0] != -1)
			error += dup2(cmd->io_redir[0], STDIN_FILENO) == -1;
		else
			error += dup2(cmdline->fds[cmd->num * 2 - 2], STDIN_FILENO) == -1;
	}
	if ((has_output_redir(cmd) && cmd->io_redir[1] != -1)
		|| cmd->num != cmdline->cmds_n - 1)
	{
		io_dup[1] = dup(STDOUT_FILENO);
		if (io_dup[1] == -1)
			return (printf("failed to dup STDOUT_FILENO\n"), -1);
		if (cmd->io_redir[1] != -1)
			error += dup2(cmd->io_redir[1], STDOUT_FILENO) == -1;
		else
			error += dup2(cmdline->fds[cmd->num * 2 + 1], STDOUT_FILENO) == -1;
	}
	return ((error != 0) * -1);
}

// Reestablishes io after builtin command exection
int	unredirect_builtin_io(int io_dup[2])
{
	if (io_dup[0] != -1 && dup2(STDIN_FILENO, io_dup[0]) == -1)
			return (ft_dprintf(2, "Failed to reestablish STDIN_FILENO\n"),
				close_valid_fds(io_dup, 2), -1);
	if (io_dup[1] != -1 && dup2(STDOUT_FILENO, io_dup[1]) == -1)
			return (ft_dprintf(2, "Failed to reestablish STDOUT_FILENO\n"),
				close_valid_fds(io_dup, 2), -1);
	return (0);
}
