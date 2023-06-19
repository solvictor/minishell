/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rng.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 22:48:56 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/17 01:01:18 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_rng(t_msh *msh)
{
	t_rng	*rng;
	size_t			random;

	rng = &msh->rng;
		
	//printf("fd_urandom --> %d\n", rng->fd_urandom);
	//printf("curr_rand ---> %zd\n", rng->curr_rand);
	//printf("multiplier --> %zd\n", rng->multiplier);
	//printf("increment ---> %zd\n", rng->increment);

	int i = 0;
	while (i < 1000)
	{
		rng->curr_rand = rng_bit_rot((rng->curr_rand + rng->increment)
			* rng->multiplier);
		random = (size_t)rng->curr_rand % RNG_MODULUS;
		printf("%zu ", random);
		++i;
	}
	printf("\n");
}
