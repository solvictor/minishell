/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 02:35:57 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/01 10:56:49 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Finds the commands and prepares environment and arguments for later execution
int	prep_cmdline(t_msh *msh, t_cmdline *cmdline, t_tokenlist *tokens)
{
	cmdline->envp = env_to_arr(msh->env);
	if (cmdline->envp == NULL)
		return (ft_dprintf(2, "fail a creer envp, debug ta mere\n"), -1);
	cmdline->paths = get_paths(msh->env);
	if (cmdline->paths == NULL) // faut pas retourner une erreur normalement faut juste faire sans
		return (ft_dprintf(2, "path malloc errror\n"), -1);
	if (do_dollar_expansions(msh, tokens) == -1) // what if unvalid variable name is given? aka starts with a number
		return (ft_dprintf(2, "Failed to expand dollars for some reason idk lol\n"), -1);
	if (merge_str_tokens(tokens) == -1)
		return (ft_dprintf(2, "Failed merge str tokens\n"), -1);
	if (make_cmds_args(cmdline) == -1)
		return (ft_dprintf(2, "Failed making argv for commands\n"), -1);
	if (pathfind_cmds(cmdline) == -1) // return what?
		return (ft_dprintf(2, "Failed pathfinding the command\n"), -1);
	// do_redirections( ) //includes heredocs
	return (0);
}

// Executes a single command
// Returns 0 for success or -1 in case of error
static int	exec_cmd(t_msh *msh, t_cmd *cmd, t_cmdline *cmdline)
{
	pid_t	pid;

	if (cmd->builtin)
	{
		cmd->builtin(msh, cmd->args);
		return (0); // return builtin code here maybe
	}
	if (cmd->empty)
		return (0); // will cause problems with the wait loop since no child was created, consider still creating a child
					// or not idk
	pid = fork();
	if (pid == -1)
		return (printf("failed to make child process\n"), -1);
	else if (pid == 0)
	{
		if (cmd->path == NULL)
			return (ft_dprintf(STDOUT_FILENO, "minishellllll: %s: command pas trouve lol\n"), 0);
		if (execve(cmd->path, cmd->args, cmdline->envp) == -1)
		{
			printf("replaced return with exit but didn't free anything, investigate failure behavior\n");
			exit(EXIT_FAILURE);
			//return (printf("Failed to execute command, what do?\n"), -1); // what about path == NULL, should I let it run or print manually?
				// i think im supposed to exit here with EXIT_FAILURE
		}
	}
	else
		if (waitpid(pid, NULL, 0) == -1) // store statlock to return good number
			return (printf("Failed to waitpid, returned -1\n"), -1);
	return (0); // NO return stat lock or something
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
			exec_cmd(msh, &cmdline->cmds[i], cmdline); // protect, or maybe dont need if exit already in exec_cmd like right now
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
		ret = exec_cmd(msh, &cmdline->cmds[0], cmdline); // return what?
		if (ret == -1)
			return (ft_dprintf(2, "Failed exec_cmd with one command, DONT KNOW WHAT TO DO!!\n"), -1);
		//set msh->ret to something here
	}
	else
	{
		ret = exec_pipeline(msh, cmdline, tokens);
		if (ret == -1)
			return (ft_dprintf(2, "Failed exec_cmd with one command, DONT KNOW WHAT TO DO!!\n"), -1);
		//set msh->ret to last ret of pipeline here
	}
	return (0);
}
