/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rng.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 22:48:56 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/19 19:03:54 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_heredoc_name(char *filename, unsigned int rand)
{
	int	offset;
	int	i;

	offset = 12;
	i = 10;
	while (rand != 0)
	{
		filename[offset + i--] = '0' + rand % 10;
		rand /= 10;
	}
	while (i > 0)
		filename[offset + i--] = '0';
}

void	test_rng(t_msh *msh)
{
	t_rng		*rng;
	static char	heredoc_name[] = ".msh_heredoc_0000000000\0";

	rng = &msh->rng;

	int i = 0;
	while (i < 10)
	{
		rng->rand = rng_bit_rot((rng->rand + rng->inc) * rng->mult);
		set_heredoc_name(heredoc_name, rng->rand);
		printf("%s\n", heredoc_name);
		++i;
	}
	// REMOVE COMMENTS AFTER THIS FUNCTION, NORMINETTE DOESN'T CATCH THIS ERROR
}

	//printf("fd_urandom --> %d\n", rng->fd_urandom);
	//printf("curr_rand ---> %zd\n", rng->curr_rand);
	//printf("multiplier --> %zd\n", rng->multiplier);
	//printf("increment ---> %zd\n", rng->increment);

	//printf("%u\n", (unsigned int)-1);
	//set_heredoc_name(heredoc_name, 1);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 0);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 9);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 10);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 11);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 100);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 101);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 99);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 4294967295);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, (unsigned int)4294967296);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, (unsigned int)4294967297);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2147483647);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2147483648);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2147483649);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 69201337);
	//printf("%s\n", heredoc_name);
