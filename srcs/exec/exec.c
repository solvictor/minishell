/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 02:35:57 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/25 13:57:23 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_cmd(t_msh *msh, t_cmd *cmd, t_cmdline *cmdline)
{
	int	pid;

	(void)msh; // only added for builtin prototype
	if (cmdline->cmds_n == 1)
	{
		if (cmd->builtin != NULL)
		{
			cmd->builtin(msh, cmd->args);
			return (0);
		}
		pid = fork();
		if (pid == -1)
			return (printf("failed to make child process\n"), -1);
		else if (pid == 0)
		{
			if (execve(cmd->path, cmd->args, cmdline->envp) == -1)
				return (printf("Failed to execute command, what do?\n"), -1);
		}
		else
		{
			//printf("parent is waiting...\n");
			waitpid(pid, NULL, 0);
			//printf("parent done waiting.\n");
		}
	}
	else
		printf("pas faire encore sa exec_cmd pour plusieurs commandes\n");
	return (0);
}

// Finds the commands and prepares environment and arguments for later execution
int	prep_cmdline(t_msh *msh, t_cmdline *cmdline, t_tokenlist *tokens)
{
	cmdline->envp = env_to_arr(msh->env);
	if (cmdline->envp == NULL)
		return (ft_dprintf(2, "fail a creer envp, debug ta mere\n"), -1);
	cmdline->paths = get_paths(msh->env);
	if (cmdline->paths == NULL) // faut pas retourner une erreur normalement faut juste faire sans
		return (ft_dprintf(2, "path malloc errror\n"), -1);
	if (do_dollar_expansions(msh, tokens) == -1)
		return (ft_dprintf(2, "Failed to expand dollars for some reason idk lol\n"), -1);
	if (merge_str_tokens(tokens) == -1)
		return (ft_dprintf(2, "Failed merge str tokens\n"), -1);
	if (make_cmds_args(cmdline) == -1)
		return (ft_dprintf(2, "Failed making argv for commands\n"), -1);
	if (pathfind_cmds(cmdline) == -1) // return what?
		return (ft_dprintf(2, "Failed pathfinding the command\n"), -1);
	return (0);
}

// Executes every function in the command line
// In case of a pipeline, commands are executed in a subshell
int	exec_cmdline(t_msh *msh, t_cmdline *cmdline)
{
	int		ret;

	(void)msh; // might not even need msh here maybe
	ret = exec_cmd(msh, &cmdline->cmds[0], cmdline); // return what?
	if (ret == -1)
		ft_dprintf(2, "DONT KNOW WHAT TO DO LOL\n");
	//make_subshell(); // for pipeline do something like this in a loop
	return (0);
}
