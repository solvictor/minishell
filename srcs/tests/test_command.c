/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:29:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/20 19:32:20 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type >= HEREDOC && token->type <= OUTPUTFILE_APPEND);
}

static int	is_str_token(t_tokenlist *token)
{
	if (token == NULL)
		return (0);
	return (token->type <= MERGED_STR);
}

static int	clean_redir_tokens(t_tokenlist **tokens)
{
	t_tokenlist *cur;
	t_tokenlist **prev_next_ptr;

	prev_next_ptr = tokens;
	cur = *tokens;
	while (cur)
	{
		if (!is_redir_token(cur))
		{
			prev_next_ptr = &cur->next;
			cur = cur->next;
		}
		else
		{
			cur->next->type = cur->type;
			*prev_next_ptr = cur->next;
			prev_next_ptr = &cur->next->next;
			free(cur);
			cur = *prev_next_ptr;
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
	while (cur && cur->type <= OUTPUTFILE_APPEND)
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
	while (cur && cur->type <= OUTPUTFILE_APPEND)
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

//static int	set_cmd(t_msh *msh, t_cmd *cmd)
//{
//	char **paths;
//	// expand dollars
//	
//	// merge strings
//	//if (merge_str_tokens(cmd) == -1) // gonna be annoying, unless tokens has a prev pointer
//	//	return (-1);
//	
//	// transform tokens in argv (ignore redirections)
//	if (set_cmd_args(cmd) == NULL)
//		return (-1);
//
//	//display_cmd_args(cmd);
//	
//	// transform env list to array
//	cmd->env = env_to_arr(msh->env);
//	if (cmd->env == NULL)
//		return (free(cmd->args), -1);
//	
//	// find command path and store in path var
//	paths = get_paths(msh->env);
//	if (paths == NULL)
//		return (free(cmd->args), free(cmd->env), -1);
//	if (find_cmd(paths, cmd) == -1)
//		return (free(cmd->args), free(cmd->env), clear_strarr(paths), -1);
//	return (0);
//}

//static int	exec_cmd(t_msh *msh, t_cmd *cmd)
//{
//	// do every heredoc and set redirection to final one, free and clean unused heredocs (ignore for now)
//	
//}

static int	check_syntax_errors(t_tokenlist *tokens)
{
	t_tokenlist *cur;

	if (tokens == NULL)
		return (printf("NULL tokens, not a syntax error\n"), -1);
	cur = tokens;
	while (cur)
	{
		if (is_redir_token(cur) && !is_str_token(cur->next))
			return (printf("redir syntax error, print context...\n"), -1); // print something
		if (cur->type == PIPE && (cur == tokens || cur->next == NULL
				|| (cur->next && cur->next->type == PIPE))) // remove `&&` and all other bonus tokens and treat them as normal text
			return (printf("double pipes or pipe at beginning, print context...\n"), -1);
		if (cur->type == UNKNOWN) // didn't check if it's even possible for UNKNOWN token to appear here
			return (printf("unknown token, idk\n"), -1);
		cur = cur->next;
	}
	return (0);
}

static void	display_pipeline(t_pipeline *pipeline)
{
	int	i;
	t_tokenlist *cur;

	printf("number of commands -> %d\n", pipeline->cmd_count);
	if (pipeline->cmds == NULL)
		return ((void)printf("pas encore malloc ta maman\n"));
	i = 0;
	while (i < pipeline->cmd_count)
	{
		printf("COMMAND #%d\n", i);
		cur = pipeline->cmds[i].start_token;
		while (cur && cur->type < PIPE)
		{
			if (is_str_token(cur))
				printf("%s ", cur->data);
			else if (cur->type == HEREDOC)
				printf("<< %s ", cur->data);
			else if (cur->type == INPUTFILE)
				printf("< %s ", cur->data);
			else if (cur->type == OUTPUTFILE_TRUNC)
				printf("> %s ", cur->data);
			else if (cur->type == OUTPUTFILE_APPEND)
				printf(">> %s ", cur->data);
			else
				printf("WAGNAGNAGNAALSKDLKFJAAOIEPW\n");
			cur = cur->next;
		}
		printf("\n");
		++i;
	}
}

static int	count_pipeline_commands(t_tokenlist *tokens)
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

static void	set_cmds_start_token(t_pipeline *pipeline, t_tokenlist *tokens)
{
	t_tokenlist	*cur;
	int			count;

	cur = tokens;
	count = 0;
	while (1)
	{
		pipeline->cmds[count].start_token = cur;
		while (cur && cur->type < PIPE)
			cur = cur->next;
		++count;
		if (cur && cur->type == PIPE)
			cur = cur->next;
		else
			break;
	}
}

static int	set_pipeline(t_pipeline *pipeline, t_tokenlist *tokens)
{
	pipeline->cmd_count = count_pipeline_commands(tokens);
	pipeline->cmds = malloc(sizeof(t_cmd) * pipeline->cmd_count);
	if (pipeline->cmds == NULL)
		return (printf("failed to malloc cmds\n"), -1);
	ft_bzero(pipeline->cmds, sizeof(t_cmd) * pipeline->cmd_count);
	set_cmds_start_token(pipeline, tokens);
	return (0);
}

void	test_command(t_msh *msh)
{
	int			ret;
	t_tokenlist	*tokens;
	t_pipeline	pipeline;

	pipeline.cmds = NULL; // remove

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return ((void)printf(MSH_ERROR ME_TOKENIZE));

	if (check_syntax_errors(tokens) == -1 || clean_redir_tokens(&tokens) == -1)
		return (printf("Syntax error return\n"), destroy_tokenlist(&tokens));
	//display_tokens(tokens);

	if (set_pipeline(&pipeline, tokens) == -1)
		return (printf("failed to set pipeline\n"), destroy_tokenlist(&tokens));
	//my_cmd.start_token = tokens;
	//if (set_cmd(msh, &my_cmd) == -1)
	//	return (printf("couldn't set command\n"), destroy_tokenlist(&tokens));
	display_pipeline(&pipeline);

	//free(my_cmd.args);
	//free(my_cmd.env);
	free(pipeline.cmds);
	destroy_tokenlist(&tokens);
}
