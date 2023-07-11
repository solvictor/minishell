/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 12:00:33 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/11 19:10:37 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_context	g_context;

// Redirects the input and output in case for interactive and non-interactive
// modes
// Returns 0 on success and -1 otherwise
//static int	get_input(char **input)
//{
//	int	fd;
//
//	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
//		return ((*input = readline(MSH_PROMPT)), 0);
//	if (!isatty(STDOUT_FILENO))
//	{
//		fd = dup(STDOUT_FILENO);
//		if (fd == -1)
//			return (-1);
//		if (dup2(STDERR_FILENO, STDOUT_FILENO) == -1)
//			return (close(fd), -1);
//		*input = readline(MSH_PROMPT);
//		if (dup2(fd, STDOUT_FILENO) == -1)
//			return (close(fd), -1);
//		return (close(fd), 0);
//	}
//	fd = dup(STDOUT_FILENO);
//	if (fd == -1)
//		return (-1);
//	close(STDOUT_FILENO);
//	*input = readline(NULL);
//	if (dup2(fd, STDOUT_FILENO) == -1)
//		return (close(fd), -1);
//	return (close(fd), 0);
//}
static int	get_input(char **input)
{
	int	fds[2];

	fds[1] = dup(STDERR_FILENO);
	if (fds[1] == -1)
		return (-1);

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		return ((*input = readline(MSH_PROMPT)), 0);
	if (isatty(STDIN_FILENO))
		*input = readline(MSH_PROMPT);
	else
	{
		fd = dup(STDOUT_FILENO); // protect 
		close(STDOUT_FILENO); // protect 
		*input = readline(NULL);
		dup2(fd, STDOUT_FILENO); // protect
		close(fd);
	}
	
	if (dup2(fd, STDERR_FILENO) == -1)
		return (close(fd), -1);
	return (close(fd), 0);
}

// Used the input and figures out the structure of commands and executes them
// Returns the output of the last command on success, -1 otherwise
static int	process_input(t_msh *msh, char *input)
{
	int	ret;

	ret = tokenize(input, &msh->tokens);
	if (ret == -1)
		return (2);
	if (parse(&msh->cmdline, msh->tokens) == -1)
		return (destroy_tokenlist(&msh->tokens), 2);
	ret = prep_cmdline(msh, &msh->cmdline, msh->tokens);
	if (ret != 0)
		return (clear_cmdline(&msh->cmdline),
			destroy_tokenlist(&msh->tokens), ret);
	ret = exec_cmdline(msh);
	return (clear_cmdline(&msh->cmdline), unlink_heredocs(msh->tokens),
		destroy_tokenlist(&msh->tokens), ret);
}

// Input loop
// Parses inputs, executes commands and updates history
static void	msh_loop(t_msh *msh)
{
	char	*input;

	while (!msh->exit)
	{
		if (get_input(&input) == -1)
			return (perror("minishell: get_input:"),
				(void)builtin_exit(msh, (char *[]){"exit\0", NULL}));
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
