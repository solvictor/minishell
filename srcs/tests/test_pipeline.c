/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:15:09 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/25 14:18:20 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char **get_command_args(const t_tokenlist *tokens)
{
	t_tokenlist *curr;
	int			i;

	curr = tokens;
	while (curr && curr->type <= UNQUOTED_STR)

}

static t_cmd	*get_command(const t_tokenlist *tokens)
{
	t_cmd		*tmp;

	if (!tokens || tokens->type != STR)
		return (NULL);
	tmp = malloc(sizeof(t_cmd));
	if (tmp == NULL)
		return (NULL);
	tmp->path = tokens->data;
	tmp->args = get_command_args(tokens->next);
	if (tmp->args == NULL)
		return (free(tmp), NULL);
	return (tmp);
}

static void	display_command(const t_cmd *cmd) // REMOOOOOOOOOOOOOOOOOOOOOOOOOOOVE LATER
{
	int	i;

	if (cmd == NULL || cmd->path == NULL || cmd->args == NULL)
		return ((void)printf("Invalid cmd, can't print\n"));
	i = 0;
	while (cmd->args[i])
		printf("%s --- ", cmd->args[i]);
	printf("\n");
}

void	test_pipeline(t_msh *msh)
{
	t_tokenlist	*tokens;
	t_cmd		*cmd;

	if (tokenize(msh, msh->input, &tokens) == -1)
		return ((void)printf("Failed to tokenize input\n"));
	cmd = get_command(tokens);
	if (cmd == NULL)
		return ((void)printf("Failed to get command\n"));
	display_command(cmd);
}
