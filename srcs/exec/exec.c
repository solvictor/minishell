/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 02:35:57 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/16 03:51:53 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

// Executes a single builtin function
// Needs its own function because of redirections
static int	exec_builtin(t_msh *msh, t_cmd *cmd)
{
	int	io_dup[2];
	int	ret;

	set_int_array(io_dup, -1, 2);
	if (redirect_builtin_io(&msh->cmdline, cmd, io_dup) == -1)
		return (close_valid_fds(io_dup, 2), 1);
	ret = cmd->builtin(msh, cmd->args);
	if (unredirect_builtin_io(io_dup) == -1)
		return (1);
	return (ret);
}

static void	newline_ifsignaled_last(t_msh *msh, t_cmd *cmd, int stat_loc)
{
	if (cmd->num == msh->cmdline.cmds_n - 1
		&& get_exit_status(stat_loc) == 130 && WIFSIGNALED(stat_loc))
	{
		ft_printf("\n");
		rl_on_new_line();
	}
}

// Executes a single command
// Returns 0 for success or a positive integer in case of error
static int	exec_cmd(t_msh *msh, t_cmd *cmd)
{
	pid_t	pid;
	int		stat_loc;

	if (cmd->empty)
		return (0);
	if ((has_input_redir(cmd) && cmd->redirs[0] == -1)
		|| (has_output_redir(cmd) && cmd->redirs[1] == -1))
		return (2);
	if (cmd->builtin)
		return (exec_builtin(msh, cmd));
	if (cmd->path == NULL)
		return (ft_dprintf(STDERR_FILENO, "minishell: %s: command not found\n",
				cmd->args[0]), 127);
	pid = fork();
	if (pid == -1)
		return (2);
	else if (pid == 0)
		child_process(msh, cmd);
	close_valid_fds(msh->cmdline.pipes, msh->cmdline.cmds_n * 2);
	close_valid_fds(msh->cmdline.redirs, msh->cmdline.cmds_n * 2);
	waitpid(pid, &stat_loc, 0);
	newline_ifsignaled_last(msh, cmd, stat_loc);
	return (get_exit_status(stat_loc));
}

// Executes all commands of a pipeline at the same time
// Returns 0 for success or a positive integer in case of error
static	int	exec_pipeline(t_msh *msh)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < msh->cmdline.cmds_n)
	{
		pid = fork();
		if (pid == -1)
			return (kill_children(&msh->cmdline, i), 2);
		else if (pid == 0)
		{
			g_context.cur = CONT_CHILD_FORK;
			msh_terminate(msh, exec_cmd(msh, &msh->cmdline.cmds[i]));
		}
		msh->cmdline.pids[i] = pid;
		++i;
	}
	close_valid_fds(msh->cmdline.pipes, msh->cmdline.cmds_n * 2);
	close_valid_fds(msh->cmdline.redirs, msh->cmdline.cmds_n * 2);
	return (wait_pipeline(pid, i));
}

// Executes every function in the command line
// In case of a pipeline, commands are executed in a subshell
int	exec_cmdline(t_msh *msh)
{
	int	ret;

	g_context.cur = CONT_PARENT_WAIT;
	if (msh->cmdline.cmds_n == 1)
		ret = exec_cmd(msh, &msh->cmdline.cmds[0]);
	else
		ret = exec_pipeline(msh);
	g_context.cur = CONT_PARENT;
	return (ret);
}
