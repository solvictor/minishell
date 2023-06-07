/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 12:29:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/07 22:59:42 by nlegrand         ###   ########.fr       */
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

// set_heredoc_token()
// set_inputfile_token()
// set_redir_trunc_token()
// set_redir_append_token()

//static int	clean_arrow_tokens(t_tokenlist **tokens)
//{
//	t_tokenlist *curr;
//	t_tokenlist **prev;
//
//	prev = tokens;
//	curr = *tokens;
//	while (curr)
//	{
//		if (curr->type == L_ARROW_DOUBLE &&
//			set_heredoc_token(curr, prev, &prev) == -1)
//			return (-1);
//		//else if (curr->type == R_ARROW_DOUBLE &&
//		//	set_redir_append( ) == -1)
//		//	return (-1);
//		//else if (curr->type == L_ARROW_SINGLE &&
//		//	set_inputfile_token( ) == -1)
//		//	return (-1);
//		//else if (curr->type == R_ARROW_SINGLE &&
//		//	set__token( ) == -1)
//		//	return (-1);
//	}
//	return (0);
//}

//static int	set_cmd(t_msh *msh, t_cmd *cmd)
//{
//	// expand dollars
//	// transform tokens in argv (and merge strings) (ignore heredocs)
//	// find command path and store in path var
//}
//
//static int	exec_cmd(t_msh *msh, t_cmd *cmd)
//{
//	// do every heredoc and set redirection to final one, free and clean unused heredocs (ignore for now)
//	// 
//}

void	test_command(t_msh *msh)
{
	int			ret;
	t_tokenlist	*tokens;
	//t_cmd		my_cmd;

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return ((void)printf(MSH_ERROR ME_TOKENIZE));

	display_tokens(tokens);
	if (clean_redir_tokens(&tokens) == -1)
		return (printf("Wrong redirection without delimiter probably\n"),
			destroy_tokenlist(&tokens));
	display_tokens(tokens);

	//my_cmd.start_token = tokens;
	//if (set_cmd(msh, &my_cmd) == -1)
	//	return (printf("couldn't set command\n"), destroy_tokenlist(&tokens));
	//if (exec_cmd(msh, &my_cmd) == -1)
	//	return (printf("couldn't exec command\n"), destroy_tokenlist(&tokens));

	destroy_tokenlist(&tokens);
}
