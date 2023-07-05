/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_redirections_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 11:31:12 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 18:52:39 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Opens input file and stores fd in the redirections array
// Returns 0 on success, -1 otherwise
int	do_redir_input(t_cmd *cmd, t_tokenlist *token)
{
	static char	heredoc_name[] = ".msh_heredoc_0000000000\0";

	close_valid_fds(&cmd->redirs[0], 1);
	if (token->type == HEREDOC)
		cmd->redirs[0] = open(set_heredoc_name(heredoc_name, token->data_opt),
				O_RDONLY, 0644);
	else
		cmd->redirs[0] = open(token->data, O_RDONLY, 0644);
	if (cmd->redirs[0] == -1)
		return (ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
				token->data, strerror(errno)), -1);
	return (0);
}

// Opens output file and stores fd in the redirections array
// Returns 0 on success, -1 otherwise
int	do_redir_output(t_cmd *cmd, t_tokenlist *token)
{
	close_valid_fds(&cmd->redirs[1], 1);
	if (token->type == O_FILE_TRUNC)
		cmd->redirs[1] = open(token->data, O_CREAT | O_TRUNC | O_WRONLY,
				0644);
	else
		cmd->redirs[1] = open(token->data, O_CREAT | O_APPEND | O_WRONLY,
				0644);
	if (cmd->redirs[1] == -1)
		return (ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
				token->data, strerror(errno)), -1);
	return (0);
}
