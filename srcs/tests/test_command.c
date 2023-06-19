/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:29:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/15 01:19:22 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type >= HEREDOC && token->type <= OUTPUT_APPEND);
}

static int	is_str_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type <= SINGLE_QUOTED_STR);
}

static int	clean_redir_tokens(t_tokenlist **tokens)
{
	t_tokenlist *cur;
	t_tokenlist **prev;

	prev = tokens;
	cur = *tokens;
	while (cur)
	{
		if (!is_redir_token(cur))
		{
			prev = &cur->next;
			cur = cur->next;
		}
		else if (!is_str_token(cur->next))
				return (-1);
		else
		{
			cur->next->type = cur->type;
			*prev = cur->next;
			prev = &cur->next->next;
			free(cur);
			cur = *prev;
		}
	}
	return (0);
}

static char **set_cmd_args(t_cmd *cmd)
{
	t_tokenlist	*cur;
	int			arg_count;

	cur = cmd->start_token;
	arg_count = 0;
	while (cur && cur->type <= OUTPUT_APPEND)
	{
		if (cur->type <= MERGED_STR)
			++arg_count;
		cur = cur->next;
	}
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (cmd->args == NULL)
		return (NULL);
	cmd->args[arg_count] = NULL;
	arg_count = 0;
	cur = cmd->start_token;
	while (cur && cur->type <= OUTPUT_APPEND)
	{
		if (cur->type <= MERGED_STR)
			cmd->args[arg_count++] = cur->data;
		cur = cur->next;
	}
	return (cmd->args);
}

//static void	display_cmd_args(t_cmd *cmd) // REMOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOVE LATER
//{
//	int	i;
//
//	if (cmd == NULL)
//		return ((void)printf("cmd is NULL\n"));
//	if (cmd->args == NULL)
//		return ((void)printf("cmd->args is NULL\n"));
//	i = 0;
//	while (cmd->args[i])
//		printf("%s ", cmd->args[i++]);
//	printf("\n");
//}

static int	set_cmd(t_msh *msh, t_cmd *cmd)
{
	char **paths;
	// expand dollars
	
	// merge strings
	//if (merge_str_tokens(cmd) == -1) // gonna be annoying, unless tokens has a prev pointer
	//	return (-1);
	
	// transform tokens in argv (ignore redirections)
	if (set_cmd_args(cmd) == NULL)
		return (-1);

	//display_cmd_args(cmd);
	
	// transform env list to array
	cmd->env = env_to_arr(msh->env);
	if (cmd->env == NULL)
		return (free(cmd->args), -1);
	
	// find command path and store in path var
	paths = get_paths(msh->env);
	if (paths == NULL)
		return (free(cmd->args), free(cmd->env), -1);
	if (find_cmd(paths, cmd) == -1)
		return (free(cmd->args), free(cmd->env), clear_strarr(paths), -1);
	return (0);
}

//static int	exec_cmd(t_msh *msh, t_cmd *cmd)
//{
//	// do every heredoc and set redirection to final one, free and clean unused heredocs (ignore for now)
//	
//}

void	test_command(t_msh *msh)
{
	int			ret;
	t_tokenlist	*tokens;
	t_cmd		my_cmd;

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return ((void)printf(MSH_ERROR ME_TOKENIZE));

	if (clean_redir_tokens(&tokens) == -1)
		return (printf("Wrong redirection without delimiter probably\n"),
			destroy_tokenlist(&tokens));
	//display_tokens(tokens);

	my_cmd.start_token = tokens;
	if (set_cmd(msh, &my_cmd) == -1)
		return (printf("couldn't set command\n"), destroy_tokenlist(&tokens));

	//if (exec_cmd(msh, &my_cmd) == -1)
	//	return (printf("couldn't exec command\n"), destroy_tokenlist(&tokens));

	free(my_cmd.args);
	free(my_cmd.env);
	destroy_tokenlist(&tokens);
}
