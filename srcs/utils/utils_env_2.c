/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 01:34:42 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/11 01:41:42 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Updates the PWD and OLDPWD environment variables after changing directory
void	set_pwds(t_msh *msh)
{
	char	*pwd;
	char	*var;

	pwd = get_env_val(msh->env, "PWD");
	if (pwd != NULL)
	{
		var = ft_strjoin("OLDPWD=", pwd);
		if (var != NULL)
		{
			builtin_export(msh, (char *[]){"export", var, NULL});
			free(var);
		}
	}
	pwd = getcwd(NULL, 0);
	var = ft_strjoin("PWD=", pwd);
	if (var == NULL)
		return (free(pwd));
	builtin_export(msh, (char *[]){"export", var, NULL});
	free(pwd);
	free(var);
}
