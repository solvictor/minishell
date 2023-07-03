/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 20:10:06 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 10:13:35 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Transforms the token list into a struct of cmdline
int	parse(t_cmdline *cmdline, t_tokenlist *tokens)
{
	cmdline->cmds_n = count_cmdline_commands(tokens);
	cmdline->cmds = malloc(sizeof(t_cmd) * cmdline->cmds_n);
	if (cmdline->cmds == NULL)
		return (printf("failed to malloc cmds in cmdline\n"), -1);
	ft_bzero(cmdline->cmds, sizeof(t_cmd) * cmdline->cmds_n);
	cmdline->fds = malloc(sizeof(int) * (cmdline->cmds_n * 2));
	if (cmdline->fds == NULL)
		return (printf("failed to malloc fds in cmdline\n"), -1);
	set_int_array(cmdline->fds, -1, cmdline->cmds_n * 2);
	set_cmds_start_token(cmdline, tokens);
	cmdline->paths = NULL;
	cmdline->envp = NULL;
	return (0);
}
