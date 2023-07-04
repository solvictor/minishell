/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 13:23:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/04 13:03:11 by nlegrand         ###   ########.fr       */
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
		if (cur->type == HEREDOC || cur->type == I_FILE)
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
		if (cur->type == O_FILE_TRUNC || cur->type == O_FILE_APPEND)
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
	if (cmd->redirs[0] != -1)
		error += dup2(cmd->redirs[0], STDIN_FILENO) == -1;
	else if (cmd->num != 0)
		error += dup2(cmdline->pipes[cmd->num * 2 - 2], STDIN_FILENO) == -1;
	if (cmd->redirs[1] != -1)
		error += dup2(cmd->redirs[1], STDOUT_FILENO) == -1;
	else if (cmd->num != cmdline->cmds_n - 1)
		error += dup2(cmdline->pipes[cmd->num * 2 + 1], STDOUT_FILENO) == -1;
	close_valid_fds(cmdline->pipes, cmdline->cmds_n * 2);
	close_valid_fds(cmdline->redirs, cmdline->cmds_n * 2);
	return ((error != 0) * -1);
}

// Redirects inputs and outputs for builtin commands and saves the old io in
// io_dup to reestablish it later
int	redirect_builtin_io(t_cmdline *cmdline, t_cmd *cmd, int io_dup[2])
{
	int	error;

	error = 0;
	if (cmd->redirs[0] != -1 || cmd->num != 0)
	{
		io_dup[0] = dup(STDIN_FILENO);
		if (io_dup[0] == -1)
			return (printf("failed to dup STDIN_FILENO\n"), -1);
		if (cmd->redirs[0] != -1)
			error += dup2(cmd->redirs[0], STDIN_FILENO) == -1;
		else
			error += dup2(cmdline->pipes[cmd->num * 2 - 2], STDIN_FILENO) == -1;
	}
	if (cmd->redirs[1] != -1 || cmd->num != cmdline->cmds_n - 1)
	{
		io_dup[1] = dup(STDOUT_FILENO);
		if (io_dup[1] == -1)
			return (printf("failed to dup STDOUT_FILENO\n"), -1);
		if (cmd->redirs[1] != -1)
			error += dup2(cmd->redirs[1], STDOUT_FILENO) == -1;
		else
			error += dup2(cmdline->pipes[cmd->num * 2 + 1], STDOUT_FILENO) == -1;
	}
	close_valid_fds(cmdline->pipes, cmdline->cmds_n * 2);
	close_valid_fds(cmdline->redirs, cmdline->cmds_n * 2);
	return ((error != 0) * -1);
}

// Reestablishes io after builtin command exection
int	unredirect_builtin_io(int io_dup[2])
{
	int	error;
	
	error = 0;
	if (io_dup[0] != -1)
		error += dup2(STDIN_FILENO, io_dup[0]) == -1;
	if (io_dup[1] != -1)
		error += dup2(STDOUT_FILENO, io_dup[1]) == -1;
	close_valid_fds(io_dup, 2);
	return ((error != 0) * -1);
}
