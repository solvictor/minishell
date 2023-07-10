/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 12:02:53 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/10 22:28:16 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Finds the commands and prepares environment and arguments for later execution
int	prep_cmdline(t_msh *msh, t_cmdline *cmdline, t_tokenlist *tokens)
{
	int	ret;

	cmdline->envp = env_to_arr(msh->env);
	if (cmdline->envp == NULL)
		return (2);
	cmdline->paths = get_paths(msh->env, "PATH");
	if (cmdline->paths == NULL)
		return (2);
	if (merge_heredoc_tokens(tokens) == -1)
		return (2);
	if (do_dollar_expansions(msh, tokens) == -1)
		return (2);
	if (merge_str_tokens(tokens) == -1)
		return (2);
	clean_redir_tokens(tokens);
	if (make_cmds_args(cmdline) == -1)
		return (2);
	if (pathfind_cmds(cmdline) == -1)
		return (2);
	ret = do_heredocs(msh);
	if (ret != 0)
		return (ret);
	if (do_redirections(cmdline) == -1)
		return (2);
	return (0);
}
