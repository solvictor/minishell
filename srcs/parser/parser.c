/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 20:10:06 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 15:45:52 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Iterates through token list and counts the number of commands
static int	count_cmdline_commands(t_tokenlist *tokens)
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
			break ;
	}
	return (count);
}

// Finds the first token for every command and stores it in the cmdline struct
static void	set_cmds_attributes(t_cmdline *cmdline, t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	int			index;

	cur = tokens;
	index = 0;
	while (1)
	{
		cmdline->cmds[index].num = index;
		cmdline->cmds[index].start_token = cur;
		cmdline->cmds[index].redirs = &cmdline->redirs[index * 2];
		while (cur && cur->type < PIPE)
			cur = cur->next;
		++index;
		if (cur && cur->type == PIPE)
			cur = cur->next;
		else
			break ;
	}
}

// Transforms the token list into a struct of cmdline
int	parse(t_cmdline *cmdline, t_tokenlist *tokens)
{
	cmdline->cmds_n = count_cmdline_commands(tokens);
	cmdline->cmds = malloc(sizeof(t_cmd) * cmdline->cmds_n);
	if (cmdline->cmds == NULL)
		return (-1);
	ft_bzero(cmdline->cmds, sizeof(t_cmd) * cmdline->cmds_n);
	cmdline->pipes = malloc(sizeof(int) * (cmdline->cmds_n * 2));
	if (cmdline->pipes == NULL)
		return (free(cmdline->cmds), -1);
	cmdline->redirs = malloc(sizeof(int) * (cmdline->cmds_n * 2));
	if (cmdline->redirs == NULL)
		return (free(cmdline->cmds), free(cmdline->pipes), -1);
	set_int_array(cmdline->pipes, -1, cmdline->cmds_n * 2);
	set_int_array(cmdline->redirs, -1, cmdline->cmds_n * 2);
	set_cmds_attributes(cmdline, tokens);
	cmdline->paths = NULL;
	cmdline->envp = NULL;
	return (0);
}
