/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_rng.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 23:37:49 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/19 17:47:12 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static unsigned int	get_randint(t_rng *rng)
{
	static unsigned int	zero_fix = RNG_ZERO_FIX_SEED;
	unsigned int		randint;
	int					ret;

	ret = read(rng->fd_urandom, &randint, sizeof(randint));
	if (ret == -1)
		return (0);
	if (randint == 0)
	{
		zero_fix = rng_bit_rot((zero_fix - 298342394234) * 972893401234);
		randint = zero_fix;
	}
	return (randint);
}

int	init_rng(t_rng *rng)
{
	rng->fd_urandom = open("/dev/urandom", O_RDONLY);
	if (rng->fd_urandom == -1)
		return (-1);
	rng->rand = get_randint(rng);
	if (rng->rand == 0)
		return (-1);
	rng->mult = get_randint(rng);
	if (rng->mult == 0)
		return (-1);
	rng->inc = get_randint(rng);
	if (rng->inc == 0)
		return (-1);
	return (0);
}
