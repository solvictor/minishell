/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 21:53:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/24 02:50:02 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Iterates through token list and counts the number of commands
int	count_cmdline_commands(t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	int			count;

	cur = tokens;
	count = 0;
	while (1)
	{
		while (cur && cur->type < PIPE)
			cur = cur->next;
		++count;
		if (cur && cur->type == PIPE)
			cur = cur->next;
		else
			break;
	}
	return (count);
}

// Finds the first token for every command and stores it in the cmdline struct
void	set_cmds_start_token(t_cmdline *cmdline, t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	int			count;

	cur = tokens;
	count = 0;
	while (1)
	{
		cmdline->cmds[count].start_token = cur;
		while (cur && cur->type < PIPE)
			cur = cur->next;
		++count;
		if (cur && cur->type == PIPE)
			cur = cur->next;
		else
			break;
	}
}
