/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 23:37:49 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/23 08:45:11 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Outputs a pseudo-random number from the seeded values in rng
unsigned int	get_randint(t_rng *rng)
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

// Signal handling function for sigint (ctrl+c)
void	handler_sigint(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", ft_strlen(MSH_PROMPT));
		rl_redisplay();
	}
}