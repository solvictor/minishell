/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:29:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/03/27 17:48:26 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//static int	is_whitespace(const char c)
//{
//	return ((c >= 9 && c <= 13) || c == ' ');
//}
//
//static int	len_to_whitespace(const char *str)
//{
//	int i;
//
//	i = 0;
//	while (str[i] && ((str[i] < 9 && str[i] > 13) || str[i] != ' '))
//		++i;
//	return (i);
//}
//
////static int	parse_pipeline( )
////{
////
////}
//
//static int	split_args(t_cmd *cmd, const char *cmd_str)
//{
//	const int	num_args = count_args(cmd_str);
//
//	if (num_args == 0)
//		return (num_args);
//	else if (num_args == -1)
//		return (printf(MSH_ERROR ME_BAD_FORMAT), -1);
//	cmd->args = malloc(sizeof(char *) * num_args);
//}
//
//static int	find_cmd(t_cmd *cmd)
//{
//	cmd->path = ft_strdup(cmd->args[0]); // not final logic
//	if (cmd->path == NULL)
//		return (-1);
//	return (0);
//}
//
//static int parse_cmd(t_cmd *cmd, const char *cmd_str)
//{
//	cmd->path = NULL;
//	cmd->args = NULL;
//	if (split_args(cmd, cmd_str) == -1)
//		return (printf(MSH_ERROR ME_SPLIT_ARGS), -1);
//	if (args == NULL)
//		return (0);
//	if (find_cmd(cmd) == -1)
//		return (printf(MSH_ERROR ME_SPLIT_ARGS), -1); // need to free args
//	return (0);
//}

//static int exec_cmd(t_msh *msh, t_cmd *cmd,) // if NULL then command wasn't found
//{
//	return (0);
//}

//void	test_parsing(t_msh *msh, const char *line)
//{
////	if (parse_cmd(&msh->cmd, msh->input) == -1)
////		return ((void)printf(ME_ERROR ME_PARSE_CMD));
//	//if (exec_cmd(msh, &msh->cmd) == -1)
//	//	return ((void)printf(ME_ERROR ME_EXEC_CMD));
//
////	int	i;
////	int	j;
////
////	i = 0;
////	while (line[i])
////	{
////		while (line[i] && is_whitespace(line[i]))
////			++i;
////		j = i;
////		if (line[i] == '\0')
////			break ;
////		else if (line[i] == '"')
////		{
////			++i;
////			while (line[i] && line[i] != '"')
////				++i;
////			if (line[i] == '\0')
////				return ((void)printf("[ERROR] non-closed quotation\n"));
////			write(1, line + j, i - j + 1);
////			write(1, "$\n", 2);
////			++i;
////		}
////		else
////		{
////			i += len_to_whitespace(line + j);
////			write(1, line + j, i - j);
////			write(1, "$\n", 2);
////		}
////		else if (line[i] == '\'')
////			while (line[i] != '\'')
////				++i;
////	}
//}
