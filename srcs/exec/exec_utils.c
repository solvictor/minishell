/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 13:23:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 19:35:54 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Uses dup2 to 
int	redirect_io(t_cmdline *cmdline, t_cmd *cmd)
{
	int	error;

	error = 0;
	if (cmd->io_redir[0] != -1)
		error += dup2(cmd->io_redir[0], STDIN_FILENO) == -1;
	else if (cmd->index != 0
		&& cmdline->cmds[cmd->index - 1].io_redir[1] == -1)
		error += dup2(cmdline->fds[cmd->index * 2 - 2], STDIN_FILENO) == -1;
	if (cmd->io_redir[1] != -1)
		error += dup2(cmd->io_redir[1], STDOUT_FILENO) == -1;
	else if (cmd->index != cmdline->cmds_n - 1)
		error += dup2(cmdline->fds[cmd->index * 2 + 1], STDOUT_FILENO) == -1;
	close_valid_fds(cmdline->fds, cmdline->cmds_n * 2);
	return ((error != 0) * -1);
}

int	redirect_builtin_io(t_cmdline *cmdline, t_cmd *cmd, int io_dup[2])
{
	int	error;

	error = 0;
	if (cmd->io_redir[0] != -1 || cmd->index != 0)
	{
		io_dup[0] = dup(STDIN_FILENO);
		if (io_dup[0] == -1)
			return (printf("failed to dup STDIN_FILENO\n"), -1);
		if (cmd->io_redir[0] != -1)
			error += dup2(cmd->io_redir[0], STDIN_FILENO) == -1;
		else
			error += dup2(cmdline->fds[cmd->index * 2 - 2], STDIN_FILENO) == -1;
	}
	if (cmd->io_redir[1] != -1 || cmd->index != cmdline->cmds_n - 1)
	{
		io_dup[1] = dup(STDOUT_FILENO);
		if (io_dup[1] == -1)
			return (printf("failed to dup STDOUT_FILENO\n"), -1);
		if (cmd->io_redir[1] != -1)
			error += dup2(cmd->io_redir[1], STDOUT_FILENO) == -1;
		else
			error += dup2(cmdline->fds[cmd->index * 2 + 1], STDOUT_FILENO)
				== -1;
	}
	return ((error != 0) * -1);
}

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
