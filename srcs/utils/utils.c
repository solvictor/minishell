/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 06:26:22 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 10:52:06 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Liberates all resources allocated by minishell
void	msh_terminate(t_msh *msh)
{
	rl_clear_history();
	destroy_env_list(&msh->env);
	close(msh->rng.fd_urandom);
}

// Frees the content of a string array and the array itself
void	clear_strarr(char **arr)
{
	int	i;

	if (arr)
	{
		i = 0;
		while (arr[i])
			free(arr[i++]);
		free(arr);
	}
}

// replace with destroy_cmdline() and include free envp and clear paths
void	clear_cmdline(t_cmdline *cmdline)
{
	int	i;

	free(cmdline->envp);
	clear_strarr(cmdline->paths);
	i = 0;
	while (i < cmdline->cmds_n)
	{
		if (cmdline->cmds[i].args
			&& cmdline->cmds[i].path != cmdline->cmds[i].args[0])
			free(cmdline->cmds[i].path);
		free(cmdline->cmds[i].args);
		if (cmdline->fds[i * 2] != -1)
			close(cmdline->fds[i * 2]);
		if (cmdline->fds[i * 2 + 1] != -1)
			close(cmdline->fds[i * 2 + 1]);
		++i;
	}
	free(cmdline->cmds);
	free(cmdline->fds);
	// set everything to zero if i end up putting it in msh, same for tokenlist functions
	//cmdline->cmds_n = 0;
	//cmdline->cmds = NULL;
	//cmdline->fds = NULL;
	//cmdline->paths = NULL;
	//cmdline->envp = NULL;
}

// Bit rotation function used by the random number generation for random file
// name generation for heredocs
unsigned int	rng_bit_rot(unsigned int num)
{
	int				shift_val;
	unsigned int	shifted_right;
	unsigned int	shifted_left;

	shift_val = num % (sizeof(num) * 8);
	shifted_right = num >> shift_val;
	shifted_left = num << ((sizeof(num) * 8) - shift_val);
	return (shifted_left | shifted_right);
}

// Sets integer array to specified value
void	set_int_array(int *arr, int val, int size)
{
	int	i;

	i = 0;
	while (i < size)
		arr[i++] = val;
}
