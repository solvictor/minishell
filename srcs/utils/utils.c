/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 06:26:22 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/25 02:50:14 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	msh_terminate(t_msh *msh)
{
	rl_clear_history();
	destroy_env_list(&msh->env);
	close(msh->rng.fd_urandom);
}

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

unsigned int rng_bit_rot(unsigned int num)
{
	int				shift_val;
	unsigned int	shifted_right;
	unsigned int	shifted_left;

	shift_val = num % (sizeof(num) * 8);

	shifted_right = num >> shift_val;
	shifted_left = num << ((sizeof(num) * 8) - shift_val);
	return (shifted_left | shifted_right);
}
