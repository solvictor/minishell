/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 08:54:00 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/11 19:46:26 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

void	child_process(t_msh *msh, t_cmd *cmd)
{
	g_context.cur = CONT_CHILD_FORK;
	if (redirect_io(&msh->cmdline, cmd) == -1)
		msh_terminate(msh, 1);
	reset_signals();
	if (execve(cmd->path, cmd->args, msh->cmdline.envp) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd->path,
			strerror(errno));
		msh_terminate(msh, 126);
	}
}

// Waits for all the processes created by the pipeline and returns the status of
// the last one
int	wait_pipeline(pid_t last_pid, int n_children)
{
	int	ret;
	int	stat_loc;

	ret = 0;
	while (n_children--)
		if (wait(&stat_loc) == last_pid)
			ret = get_exit_status(stat_loc);
	return (ret);
}

// Kills previous children (quietly) in case of failure
void	kill_children(t_cmdline *cmdline, int i)
{
	while (i--)
	{
		if (cmdline->pids[i] > 0)
			kill(cmdline->pids[i], SIGINT);
	}
}

// Redirects the builtin input and output
int	redir_dup(t_cmdline *cmdline, t_cmd *cmd, int fd)
{
	int	*redir;
	int	pipe_i;

	redir = &cmd->redirs[fd == STDOUT_FILENO];
	pipe_i = cmd->num * 2 + (1 - (fd == STDIN_FILENO) * 3);
	if (redir[0] != -1)
		return (dup2(redir[0], fd) == -1);
	else
		return (dup2(cmdline->pipes[pipe_i], fd) == -1);
}

// Returns the correct exit status of a child
int	get_exit_status(int stat_loc)
{
	if (WIFEXITED(stat_loc))
		return (WEXITSTATUS(stat_loc));
	else
		return (128 + WTERMSIG(stat_loc));
}
