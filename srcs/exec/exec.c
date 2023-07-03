/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 02:35:57 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 12:16:12 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t g_running_child;

// Executes a single command
// Returns 0 for success or -1 in case of error
static int	exec_cmd(t_msh *msh, t_cmd *cmd, t_cmdline *cmdline,
t_tokenlist **tokens)
{
	pid_t	pid;

	// g_running_child = 1 here maybe??? so that it doesn't double print after ctrl+c in a builtin, but if this is done set it back to zero at the end of this function, wait this is already done
	if (cmd->empty)
		return (0);
	if (cmd->builtin)
		return (cmd->builtin(msh, cmd->args), 0); // return exit code of builtin
	if (cmd->path == NULL)
		return (ft_dprintf(STDOUT_FILENO, "minishellllll: %s: command pas trouve lol\n", cmd->args[0]), 127);
	pid = fork();
	if (pid == -1)
		return (printf("failed to make child process\n"), -1);
	else if (pid == 0)
	{
		g_running_child = 1;
		if (execve(cmd->path, cmd->args, cmdline->envp) == -1)
		{
			ft_dprintf(STDOUT_FILENO, "minishell: %s: %s\n", cmd->path, strerror(errno));
			clear_cmdline(cmdline);
			destroy_tokenlist(tokens);
			msh_terminate(msh);
			exit(126);
		}
	}
	if (waitpid(pid, NULL, 0) == -1) // store statlock to return good number
		return (printf("Failed to waitpid, returned -1\n"), -1);
	return (0); // NO!! Return stat lock or something
}

static	int	exec_pipeline(t_msh *msh, t_cmdline *cmdline, t_tokenlist **tokens)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < cmdline->cmds_n)
	{
		pid = fork();
		if (pid == -1)
			return (printf("Failed to make child process\n"), -1); // kill other children
		else if (pid == 0)
		{
			g_running_child = 1;
			exec_cmd(msh, &cmdline->cmds[i], cmdline, tokens); // protect, or maybe dont need if exit already in exec_cmd like right now
			clear_cmdline(cmdline);
			destroy_tokenlist(tokens);
			msh_terminate(msh);
			exit(EXIT_SUCCESS); //maybe put that in exec_cmd
		}
//		else
//		{
//			// nothing to do i think??
//		}
		++i;
	}
	while (i--)
		waitpid(-1, NULL, 0);
	return (0);
}

// Executes every function in the command line
// In case of a pipeline, commands are executed in a subshell
int	exec_cmdline(t_msh *msh, t_cmdline *cmdline, t_tokenlist **tokens)
{
	int		ret;

	if (cmdline->cmds_n == 1)
	{
		g_running_child = 2;
		ret = exec_cmd(msh, &cmdline->cmds[0], cmdline, tokens); // return what?
		g_running_child = 0;
		if (ret == -1)
			return (ft_dprintf(2, "Failed exec_cmd with one command, DONT KNOW WHAT TO DO!!\n"), -1);
		//set msh->ret to something here
	}
	else
	{
		g_running_child = 2;
		ret = exec_pipeline(msh, cmdline, tokens);
		g_running_child = 0;
		if (ret == -1)
			return (ft_dprintf(2, "Failed exec_cmd with one command, DONT KNOW WHAT TO DO!!\n"), -1);
		//set msh->ret to last ret of pipeline here
	}
	return (0);
}
