/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_rng.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 23:37:49 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/17 00:37:27 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_urandom(t_rng *rng)
{
	static ssize_t	zero_fix = RNG_ZERO_FIX_SEED;
	int	ret;
	ssize_t	random;

	ret = read(rng->fd_urandom, &random, sizeof(random));
	if (ret == -1)
		return (0);
	if (random == 0)
	{
		zero_fix = rng_bit_rot((zero_fix - 298342394234) * 972893401234);
		random = zero_fix;
	}
	return (random);
}

int	init_rng(t_rng *rng)
{
	rng->fd_urandom = open("/dev/urandom", O_RDONLY);
	if (rng->fd_urandom == -1)
		return (-1);
	rng->curr_rand = read_urandom(rng);
	if (rng->curr_rand == 0)
		return (-1);
	rng->multiplier = read_urandom(rng);
	if (rng->multiplier == 0)
		return (-1);
	rng->increment = read_urandom(rng);
	if (rng->increment == 0)
		return (-1);
	return (0);
}
