/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 09:56:54 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/04 14:12:14 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	make_heredoc(t_msh *msh, t_cmdline *cmdline)
{
	(void)msh;
	(void)cmdline;
	printf("doing heredoc...\n");
	// fork
	return (0);
}

int	do_heredocs(t_msh *msh, t_cmdline *cmdline)
{
	t_tokenlist	*cur;

	cur =  cmdline->cmds[0].start_token;
	while (cur)
	{
		if (cur->type == HEREDOC && make_heredoc(msh, cmdline) == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}
