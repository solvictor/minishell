/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:28:05 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/04 20:33:21 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

// Closes all file descriptors not equal to -1 int the array
void	close_valid_fds(int	*arr, int size)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (i < size)
	{
		if (arr[i] != -1)
		{
			close(arr[i]);
			arr[i] = -1;
		}
		++i;
	}
}

// Outputs a random number generated by the rng
unsigned int	randuint_rng(t_rng *rng)
{
	return (rng->rand = rng_bit_rot((rng->rand + rng->inc) * rng->mult));
}

// Loops through the token list to find HEREDOC tokens and unlinks them
void	unlink_heredocs(t_tokenlist *tokens)
{
	t_tokenlist *cur;
	static char	heredoc_name[] = ".msh_heredoc_0000000000\0";

	cur = tokens;
	while (cur)
	{
		if (cur->type == HEREDOC && cur->data_opt != -1
			&& g_context.n == CONT_PARENT)
			unlink(set_heredoc_name(heredoc_name, cur->data_opt));
		cur = cur->next;
	}
}

void	set_context(t_msh *msh)
{
	g_context.msh = msh;
	g_context.n = CONT_PARENT;
	g_context.heredoc_fd = -1;
}
