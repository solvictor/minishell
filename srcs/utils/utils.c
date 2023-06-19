/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 06:26:22 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/17 00:50:14 by nlegrand         ###   ########.fr       */
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

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

size_t	rng_bit_rot(size_t num)
{
	int		shift_val;
	size_t	shifted_right;
	size_t	shifted_left;

	shift_val = RNG_BIT_ROTATIONS % (sizeof(num) * 8);

	shifted_right = num >> shift_val;
	shifted_left = num << ((sizeof(num) * 8) - shift_val);
	return (shifted_left | shifted_right);
}
