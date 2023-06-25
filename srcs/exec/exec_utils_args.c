/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 09:58:53 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/25 06:02:31 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// replace with destroy_cmdline() and include free envp and clear paths
void	clear_cmdline(t_cmdline *cmdline)
{
	int	i;

	free(cmdline->envp);
	clear_strarr(cmdline->paths);
	i = 0;
	while (i < cmdline->cmds_n)
	{
		free(cmdline->cmds[i].args);
		free(cmdline->cmds[i].path);
		++i;
	}
	free(cmdline->cmds);
	ft_bzero(cmdline, sizeof(t_cmdline));
}

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
