/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:00:33 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/25 13:56:58 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Unimportant function :]
int	main(int ac, char **av, char **envp)
{
	t_msh	msh;

	(void)av;
	if (msh_setup(&msh, ac, envp) == -1)
		return (1); // check if anything is allocated
	if (msh_loop(&msh) == -1)
		printf(ME_LOOP);
	msh_terminate(&msh);
	return (msh.ret);
}

// Input loop
// Parses inputs, executes commands and updates history
int	msh_loop(t_msh *msh)
{
	char	*input;

	while (!msh->exit)
	{
		if (isatty(0))
			input = readline(MSH_PROMPT);
		else
			input = readline(NULL);
		if (input == NULL) // readline manual says NULL is returned when EOF encountered on an empty line so this should handle ctrl+d properly (maybe)
			return (0);
		if (input[0]) // probably no need for msh.cmdline check because there's a return before
		{
			add_history(input);
			msh->ret = process_input(msh, input);
			if (msh->ret == -1)
				return (printf("An error occured in process_input, returning...\n"), -1);
		}
		free(input);
	}
	return (0);
}

// Used the input and figures out the structure of commands and executes them
// Returns the output of the last command on success, -1 otherwise
int	process_input(t_msh *msh, char *input)
{
	int			ret;
	t_tokenlist	*tokens;
	t_cmdline	cmdline;

	ret = tokenize(input, &tokens);
	if (ret == -1)
		return (ft_dprintf(2, MSH_ERROR ME_TOKENIZE), -1);
	if (ret == -2)
		return (ft_printf("syntax error, try again\n"), 0);
	ret = parse(&cmdline, tokens);
	if (ret == -1)
		return (ft_dprintf(2, MSH_ERROR ME_PARSE),
			destroy_tokenlist(&tokens), -1);
	ret = prep_cmdline(msh, &cmdline, tokens);
	if (ret == -1)
		return (ft_dprintf(2, MSH_ERROR ME_PREP), clear_cmdline(&cmdline),
			destroy_tokenlist(&tokens), -1);
	//display_cmdline(&cmdline);
	ret = exec_cmdline(msh, &cmdline);
	if (ret == -1)
		ft_dprintf(2, MSH_ERROR ME_EXEC);
	return (clear_cmdline(&cmdline), destroy_tokenlist(&tokens), ret); // free(pip.cmds) replace with destroy_pipeline(&pip) later when necessary
}
