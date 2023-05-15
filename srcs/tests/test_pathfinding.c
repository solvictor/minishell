/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pathfinding.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 16:11:33 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/15 19:21:28 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_args(t_msh *msh)
{
	char	**tmp;
	int		i;
	t_tokenlist *curr;

	i = 0;
	curr = msh->tokens;
	while (curr)
	{
		++i;
		curr = curr->next;
	}
	//printf("number of tokens: %d\n", i);
	tmp = malloc(sizeof(char *) * (i + 1));
	if (tmp == NULL)
		return (NULL);
	i = 0;
	curr = msh->tokens;
	while (curr)
	{
		tmp[i] = curr->data;
		//printf("%s ", tmp[i]);
		++i;
		curr = curr->next;
	}
	//printf("\n");
	tmp[i] = NULL;
	return (tmp);
}

static char	**get_paths(t_env *env)
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
	paths = ft_split(curr->var + 5, ':');
	return (paths);
}

// If the command path is local, absolute or non existant this function
// will fill the command struct instead of find_command itself
int	special_cmd_path(char **paths, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->args[0][0] == '\0')
		return (-1);
	else if ((cmd->args[0][0] == '/' || cmd->args[0][0] == '.'
			|| paths == NULL) && ++i)
		cmd->path = ft_strdup(cmd->args[0]);
	if (i && cmd->path == NULL)
		return (printf("find_cmd > ft_strdup\n"), -1);
	return (i);
}

// Joins the command name with the paths in the PATH variable to find the
// executable
int	find_cmd(char **paths, t_cmd *cmd)
{
	int		i;
	char	*slashed;

	i = special_cmd_path(paths, cmd);
	if (i > 0)
		return (0);
	else if (i == -1)
		return (-1);
	slashed = ft_strjoin("/", cmd->args[0]);
	if (slashed == NULL)
		return (printf("strjoin failed in find_cmd (1)\n"), -1);
	i = 0;
	while (paths && paths[i])
	{
		cmd->path = ft_strjoin(paths[i++], slashed);
		if (cmd->path == NULL)
			return (free(slashed), printf("strjoin failed in find_cmd (2)\n"),  -1);
		if (access(cmd->path, F_OK) == 0)
			return (free(slashed), 0);
		free(cmd->path);
	}
	return ((cmd->path = NULL), free(slashed), -1);
}

void	test_pathfinding(t_msh *msh)
{
	t_cmd		cmd;
	int			ret;
	char		**paths;
	char		**env_arr;
	t_builtin	builtin;

	ret = tokenize(msh, msh->input, &msh->tokens);
	if (ret < 0)
		return ((void)printf(MSH_ERROR ME_TOKEN_CMD));
	//display_tokens(msh->tokens);

	paths = get_paths(msh->env);
	if (paths == NULL)
		return (destroy_tokenlist(&msh->tokens), (void)printf("failed allocating paths\n"));

	cmd.args = get_args(msh);
	if (cmd.args == NULL)
		return (destroy_tokenlist(&msh->tokens), clear_strarr(paths), (void)printf("Failed to allocate args for command\n"));

	// TODO Check (valeur de retour du builtin)
	builtin = get_builtin(&cmd);
	if (builtin)
		return (builtin(msh, cmd.args), destroy_tokenlist(&msh->tokens), clear_strarr(paths), free(cmd.args), (void) 0);

	if (find_cmd(paths, &cmd) == -1)
		return (destroy_tokenlist(&msh->tokens), clear_strarr(paths), free(cmd.args), (void)printf("Failed to find command\n"));

	env_arr = env_to_arr(msh->env);
	if (env_arr == NULL)
		return (destroy_tokenlist(&msh->tokens), clear_strarr(paths), free(cmd.args), free(cmd.path), (void)printf("Failed to convert env to str array\n"));

	ret = fork();
	if (ret == -1)
		printf("Failed to fork\n");
	else if (ret == 0)
	{
		if (execve(cmd.path, cmd.args, env_arr) == -1)
		{
			printf("Failed to execve\n");
			destroy_tokenlist(&msh->tokens);
			free(cmd.args);
			free(cmd.path);
			free(env_arr);
			clear_strarr(paths);
			exit(EXIT_FAILURE);
		}
	}
	waitpid(ret, NULL, 0);
	destroy_tokenlist(&msh->tokens);
	free(cmd.args);
	free(cmd.path);
	free(env_arr);
	clear_strarr(paths);
}
