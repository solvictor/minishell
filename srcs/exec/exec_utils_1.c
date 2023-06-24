/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 04:16:08 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/24 06:42:24 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Uses the PATH variable from the env to crate a str array of executable
// locations for pathfinding
char	**get_paths(t_env *env)
{
	char	**paths;
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (ft_strncmp(curr->var, "PATH=", 5) == 0)
			break ;
		curr = curr->next;
	}
	if (curr == NULL)
		return (NULL);
	paths = ft_split(curr->var + 5, ':'); // trouver une methode pour differencier PAS de PATH et une erreur de malloc
	return (paths);
}

//int	count_cmd_args(t_tokenlist *tokens)
//{
//	t_tokenlist	*cur;
//	int			count;
//
//	cur = tokens;
//	count = 0;
//	while (cur && cur->type < PIPE)
//	{
//		if (is_str_token(cur))
//			++count;
//		cur = cur->next;
//	}
//	return (count);
//}
//
//static int	set_cmd_args(t_tokenlist *tokens)
//{
//	int	count;
//
//	count = count_cmd_args(tokens);
//	if (count = 0)
//		cm
//}
//
//int	make_cmds_args(t_cmdline *cmdline, t_tokenlist *tokens)
//{
//	int	i;
//
//	i = 0;
//	while (i < cmdline->cmds_n)
//	{
//		cmdline->cmds[i].args = get_cmd_args(cmdline->cmds[i].start_token);
//		++i;
//	}
//
//	return (0);
//}
