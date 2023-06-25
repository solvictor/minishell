/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 02:35:57 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/25 07:08:09 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_cmd(t_cmd *cmd, t_cmdline *cmdline)
{
	// make argv and merge strings in one command
	
	(void)cmd;
	(void)cmdline;
	printf("pas faire encore sa exec_cmd\n");
	return (0);
}

int	prep_cmdline(t_msh *msh, t_cmdline *cmdline, t_tokenlist *tokens)
{
	cmdline->envp = env_to_arr(msh->env);
	if (cmdline->envp == NULL)
		return (ft_dprintf(2, "fail a creer envp, debug ta mere\n"), -1);
	cmdline->paths = get_paths(msh->env);
	if (cmdline->paths == NULL) // faut pas retourner une erreur normalement faut juste faire sans
		return (ft_dprintf(2, "path malloc errror\n"),
			free(cmdline->envp),  -1);
	if (do_dollar_expansions(msh, tokens) == -1)
		return (ft_dprintf(2, "Failed to expand dollars for some reason idk lol\n"),
			clear_cmdline(cmdline), -1);
	if (merge_str_tokens(tokens) == -1)
		return (ft_dprintf(2, "Failed merge str tokens\n"),
			clear_cmdline(cmdline), -1);
	if (make_cmds_args(cmdline) == -1)
		return (ft_dprintf(2, "Failed making argv for commands\n"),
			clear_cmdline(cmdline), -1);
	if (pathfind_cmds(cmdline) == -1) // return what?
		return (ft_dprintf(2, "Failed pathfinding the command\n"),
			clear_cmdline(cmdline), -1);
	return (0);
}

int	exec_cmdline(t_msh *msh, t_cmdline *cmdline)
{
	(void)msh; // might not even need msh here maybe
	int		ret;

	// EXECUTE
	// loop execution here
	// single command execution

	ret = exec_cmd(&cmdline->cmds[0], cmdline); // return what?
	if (ret == -1)
		ft_dprintf(2, "DONT KNOW WHAT TO DO LOL\n");

	return (clear_cmdline(cmdline), 0);
}
