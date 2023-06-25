/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_pathfind.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 04:16:08 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/25 07:06:17 by nlegrand         ###   ########.fr       */
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

static char	*concat_cmd_path(char *path, char *cmd_name)
{
	char	*tmp;
	int		len_path;
	int		len;

	len_path = ft_strlen(path);
	len = len_path + ft_strlen(cmd_name) + 1;
	tmp = malloc(sizeof(char) * (len + 1));
	if (tmp == NULL)
		return (NULL);
	ft_strlcpy(tmp, path, len + 1);
	tmp[len_path] = '/';
	tmp[len_path + 1] = '\0';
	ft_strlcat(tmp, cmd_name, len + 1);
	return (tmp);
}

static int	find_cmd(t_cmd *cmd, char **paths)
{
	int		i;
	char	*tmp;

	if (ft_strchr(cmd->args[0], '/'))
	{
		cmd->path = ft_strdup(cmd->args[0]);
		return ((cmd->path == NULL) * -1);
	}
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

int	pathfind_cmds(t_cmdline *cmdline)
{
	int	i;

	i = 0;
	while (i < cmdline->cmds_n)
	{
		if (!cmdline->cmds[i].empty &&
			find_cmd(&cmdline->cmds[i], cmdline->paths) == -1)
		{
			while (i)
				free(cmdline->cmds[--i].path);
			return (-1);
		}
		++i;
	}
	return (0);
}
