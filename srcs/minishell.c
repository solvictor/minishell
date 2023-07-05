/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:00:33 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 16:46:19 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_context	g_context;

// Cool function :]
int	main(int ac, char **av, char **envp)
{
	t_msh	msh;

	(void)av;
	errno = 0;
	set_context(&msh);
	if (msh_setup(&msh, ac, envp) == -1)
		return (1);
	msh_loop(&msh);
	msh_terminate(&msh, msh.ret);
}

// Input loop
// Parses inputs, executes commands and updates history
void	msh_loop(t_msh *msh)
{
	char	*input;

	while (!msh->exit)
	{
		if (isatty(0))
			input = readline(MSH_PROMPT);
		else
			input = readline(NULL);
		if (input == NULL)
			return ((void)builtin_exit(msh, (char *[]){"exit\0", NULL}));
		if (input[0])
		{
			add_history(input);
			msh->ret = process_input(msh, input);
		}
		free(input);
	}
}

// Used the input and figures out the structure of commands and executes them
// Returns the output of the last command on success, -1 otherwise
int	process_input(t_msh *msh, char *input)
{
	int	ret;

	ret = tokenize(input, &msh->tokens);
	if (ret == -1)
		return (1);
	if (parse(&msh->cmdline, msh->tokens) == -1)
		return (destroy_tokenlist(&msh->tokens), 1);
	ret = prep_cmdline(msh, &msh->cmdline, msh->tokens);
	if (ret == -1)
		return (clear_cmdline(&msh->cmdline),
			destroy_tokenlist(&msh->tokens), 1);
	ret = exec_cmdline(msh);
	return (clear_cmdline(&msh->cmdline), unlink_heredocs(msh->tokens),
		destroy_tokenlist(&msh->tokens), ret);
}
