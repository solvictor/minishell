/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 12:02:53 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 16:17:05 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Finds the commands and prepares environment and arguments for later execution
int	prep_cmdline(t_msh *msh, t_cmdline *cmdline, t_tokenlist *tokens)
{
	cmdline->envp = env_to_arr(msh->env);
	if (cmdline->envp == NULL)
		return (-1);
	cmdline->paths = get_paths(msh->env);
	if (cmdline->paths == NULL)
		return (-1);
	if (merge_heredoc_tokens(tokens) == -1)
		return (-1);
	if (do_dollar_expansions(msh, tokens) == -1) // what if unvalid variable name is given? aka starts with a number
		return (-1);
	if (merge_str_tokens(tokens) == -1)
		return (-1);
	clean_redir_tokens(tokens);
	if (make_cmds_args(cmdline) == -1)
		return (-1);
	if (pathfind_cmds(cmdline) == -1) // return what?
		return (-1);
	if (do_heredocs(msh) == -1)
		return (-1);
	if (do_redirections(cmdline) == -1) // return what?
		return (-1);
	return (0);
}
