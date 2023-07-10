/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 23:37:49 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/10 22:39:29 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Refreshed the PWD environment variable
int	set_pwd(t_msh *msh)
{
	int		ret;
	char	*cwd;
	char	*var;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	var = ft_strjoin("PWD=", cwd);
	if (!var)
		return (free(cwd), 1);
	ret = builtin_export(msh, (char *[]){"export", var, NULL});
	free(cwd);
	free(var);
	return (ret);
}
