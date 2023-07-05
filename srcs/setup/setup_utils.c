/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 23:37:49 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 11:17:16 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

// Outputs a pseudo-random number from the seeded values in rng
unsigned int	get_randint(int fd_urandom)
{
	static unsigned int	zero_fix = RNG_ZERO_FIX_SEED;
	unsigned int		randint;
	int					ret;

	ret = read(fd_urandom, &randint, sizeof(randint));
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
void	handler(int sig)
{
	if (sig != SIGINT)
		return ;
	g_context.msh->ret = 130; // not sure i should leave that here,
							  // probably should be done with stat_lock on
							  // wait and waitpid but not sure
	if (g_context.n == CONT_PARENT || g_context.n == CONT_CHILD_WAIT)
	{
		printf("\n"); // casse au tt debut de minishell jsp pk
		rl_on_new_line();
		if (g_context.n == CONT_PARENT)
		{
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (g_context.n == CONT_HEREDOC)
		msh_terminate(g_context.msh, 130);
}
