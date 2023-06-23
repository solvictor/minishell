/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 21:53:03 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/23 00:20:33 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Iterates through token list and counts the number of commands
int	count_pipeline_commands(t_tokenlist *tokens)
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

// Finds the first token for every command and stores it in the pipeline struct
void	set_cmds_start_token(t_pipeline *pip, t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	int			count;

	cur = tokens;
	count = 0;
	while (1)
	{
		pip->cmds[count].start_token = cur;
		while (cur && cur->type < PIPE)
			cur = cur->next;
		++count;
		if (cur && cur->type == PIPE)
			cur = cur->next;
		else
			break;
	}
}
