/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:00:33 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/15 19:21:43 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Unimportant function :]
int	main(int ac, char **av, char **envp)
{
	t_msh	msh;

	(void) av;
	if (ac != 1)
		return (ft_dprintf(2, ME_USAGE), 1);
	if (msh_setup(&msh, ac, envp) == -1)
		return (1); // check if anything is allocated
	if (msh_loop(&msh) == -1)
		printf(ME_LOOP);
	// free stuff?
	msh_terminate(&msh);
	return (msh.ret);
}

// Input loop
// Parses inputs, executes commands and updates history
int	msh_loop(t_msh *msh)
{
	while (!msh->exit)
	{
		msh->input = readline(MSH_PROMPT);
		if (msh->input == NULL) // readline manual says NULL is returned when EOF encountered on an empty line so this should handle ctrl+d properly (maybe)
			return (0);
		//if (msh.cmdline && *(msh.cmdline))
		if (msh->input[0]) // probably no need for msh.cmdline check because there's a return before
		{
			add_history(msh->input); // careful about history with heredoc
//			test_parentheses(msh->cmdline, 0);
//			test_parsing(msh, msh->input);
//			test_quotes(msh, msh->input);
			//test_tokenizer(msh);
			test_pathfinding(msh);
		}
		free(msh->input);
	}
	return (0);
}
