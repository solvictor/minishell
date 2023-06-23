/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 20:10:06 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/23 00:03:55 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Transforms the token list into a struct of pipeline
int	parse(t_pipeline *pip, t_tokenlist *tokens)
{
	pip->cmds_n = count_pipeline_commands(tokens);
	pip->cmds = malloc(sizeof(t_cmd) * pip->cmds_n);
	if (pip->cmds == NULL)
		return (printf("failed to malloc cmds\n"), -1);
	ft_bzero(pip->cmds, sizeof(t_cmd) * pip->cmds_n);
	set_cmds_start_token(pip, tokens);
	return (0);
}
