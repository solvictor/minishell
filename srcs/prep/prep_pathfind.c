/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_pathfind.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 04:16:08 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/01 10:28:04 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Uses the PATH variable from the env to crate a str array of executable
// locations for pathfinding
char	**get_paths(t_env *env)
{
	char	**tmp;
	char	*path_value;

	path_value = get_env_val(env, "PATH");
	if (path_value == NULL)
	{
		tmp = malloc(sizeof(char *));
		if (tmp == NULL)
			return (NULL);
		tmp[0] = NULL;
	}
	else
	{
		tmp = ft_split(path_value, ':');
		if (tmp == NULL)
			return (NULL);
	}
	return (tmp);
}

// Allocates a string to concatenate the command name to the directories of PATH
// for existence check with access
// Returns a pointer to the newly allocated string or NULL otherwise
static char	*concat_cmd_path(char *path, char *name)
{
	char	*tmp;
	int		len_path;
	int		len;

	len_path = ft_strlen(path);
	len = len_path + ft_strlen(name) + 1;
	tmp = malloc(sizeof(char) * (len + 1));
	if (tmp == NULL)
		return (NULL);
	ft_strlcpy(tmp, path, len + 1);
	tmp[len_path] = '/';
	tmp[len_path + 1] = '\0';
	ft_strlcat(tmp, name, len + 1);
	return (tmp);
}

// If the command name is present in the list of builtins, the builtin function
// pointer of the t_cmd struct will be set accordingly and pathfinding won't be
// performed
// Returns 1 if a buitin is found, 0 otherwise
static int	get_builtin(t_cmd *cmd)
{
	static const char		*names[] = {"cd", "pwd", "exit",
		"echo", "export", "unset", "env", NULL};
	static const t_builtin	funcs[] = {builtin_cd, builtin_pwd, builtin_exit,
		builtin_echo, builtin_export, builtin_unset, builtin_env};
	int						i;
	char					*cmd_name;

	cmd_name = cmd->args[0];
	i = 0;
	while (names[i])
	{
		if (ft_strncmp(cmd_name, names[i], ft_strlen(cmd_name) + 1) == 0)
			return ((cmd->builtin = funcs[i]), 1);
		++i;
	}
	return (0);
}

// Tries to find the path a single command and stores it in each
// 'path' property of t_cmd struct
// Returns 0 on success, -1 otherwise
static int	find_cmd(t_cmd *cmd, char **paths)
{
	int		i;
	char	*tmp;

	if (ft_strchr(cmd->args[0], '/')) // TEST LOCAL EXEC PATH WONT CRASH
		return ((cmd->path = cmd->args[0]), 0);
	if (get_builtin(cmd))
		return (0);
	i = 0;
	while (paths[i])
	{
		tmp = concat_cmd_path(paths[i], cmd->args[0]);
		if (tmp == NULL)
			return (-1);
		if (access(tmp, F_OK) == 0)
			return ((cmd->path = tmp), 0);
		free(tmp);
		++i;
	}
	return (0);
}

// Tries to find the path to the all command executables and stores it in each
// 'path' property of t_cmd struct
// Returns 0 on success, -1 otherwise
int	pathfind_cmds(t_cmdline *cmdline)
{
	int	i;

	i = 0;
	while (i < cmdline->cmds_n)
	{
		//printf("coucou\n");
		//printf("cmdline->cmds[%d].empty -> %d\n", i, cmdline->cmds[i].empty);
		//printf("cmdline->cmds[%d].args -> %p\n", i, cmdline->cmds[i].args);
		if (cmdline->cmds[i].empty == 0
			&& find_cmd(&cmdline->cmds[i], cmdline->paths) == -1)
			return (-1);
		++i;
	}
	return (0);
}
