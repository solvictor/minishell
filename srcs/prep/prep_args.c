/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 09:58:53 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/01 09:13:35 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Given the start_token of each command and counts the number of argv elements
// to be allocated
// Returns the argc basically
static int	count_cmd_args(t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	int			count;

	cur = tokens;
	count = 0;
	while (cur && cur->type < PIPE)
	{
		if (is_str_token(cur))
			++count;
		cur = cur->next;
	}
	return (count);
}

// Creates the argv array one command
// Returns 0 on success, -1 otherwise
static int	set_cmd_args(t_cmd *cmd)
{
	t_tokenlist	*cur;
	int			count;
	int			i;

	count = count_cmd_args(cmd->start_token);
	if (count == 0)
		return ((cmd->empty = 1), 0);
	cmd->args = malloc(sizeof(char *) * (count + 1));
	if (cmd->args == NULL)
		return (-1);
	cmd->args[count] = NULL;
	i = 0;
	cur = cmd->start_token;
	while (cur && cur->type < PIPE)
	{
		if (is_str_token(cur))
			cmd->args[i++] = cur->data;
		cur = cur->next;
	}
	return (0);
}

// Creates the argv array for every command
// Returns 0 on success, -1 otherwise
int	make_cmds_args(t_cmdline *cmdline)
{
	int	i;

	i = 0;
	while (i < cmdline->cmds_n)
	{
		if (set_cmd_args(&cmdline->cmds[i]) == -1)
		{
			while (i)
				free(cmdline->cmds[--i].args);
			return (-1);
		}
		++i;
	}
	return (0);
}
