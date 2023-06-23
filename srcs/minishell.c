/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:00:33 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/23 08:29:19 by nlegrand         ###   ########.fr       */
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
		if (isatty(0))
			msh->input = readline(MSH_PROMPT);
		else
			msh->input = readline(NULL);
		if (msh->input == NULL) // readline manual says NULL is returned when EOF encountered on an empty line so this should handle ctrl+d properly (maybe)
			return (0);
		if (msh->input[0]) // probably no need for msh.cmdline check because there's a return before
		{
			add_history(msh->input); // careful about history with heredoc
			msh->ret = process_input(msh);
			if (msh->ret == -1)
				return (printf("An error occured in process_input, returning...\n"), -1);
		}
		free(msh->input);
	}
	return (0);
}

int	process_input(t_msh *msh)
{
	int			ret;
	t_tokenlist	*tokens;
	t_pipeline	pip;

	ret = tokenize(msh->input, &tokens);
	if (ret < 0)
		return (ft_dprintf(2, MSH_ERROR ME_TOKENIZE), -1);
	//display_tokens(tokens);

	ret = parse(&pip, tokens);
	if (ret < 0)
		return (ft_dprintf(2, MSH_ERROR ME_PARSE),
			destroy_tokenlist(&tokens), -1);
	//display_pipeline(&pip);

	return (free(pip.cmds), destroy_tokenlist(&tokens), ret); // free(pip.cmds) replace with destroy_pipeline(&pip) later when necessary
}
