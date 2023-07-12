/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 01:34:42 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/12 19:23:41 by vegret           ###   ########.fr       */
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

int	is_valid_identifier(char *str)
{
	if (!*str || *str == '=' || ft_isdigit(*str))
		return (0);
	while (*str && *str != '=')
	{
		if (*str != '_' && !ft_isalnum(*str))
			return (0);
		str++;
	}
	return (1);
}

// Weird atoi needed by SHLVL and exit
int	is_numeric(char *str, int *dst)
{
	unsigned long	num;
	int				sign;

	sign = 1;
	num = 0;
	while (*str && (((*str >= 9) && (*str <= 13)) || (*str == ' ')))
		++str;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		if ((num > LONG_MAX && sign == 1)
			|| (num - 1 > LONG_MAX && sign == -1))
			return (0);
		str++;
	}
	while (*str && (((*str >= 9) && (*str <= 13)) || (*str == ' ')))
		++str;
	if (*str != '\0')
		return (0);
	*dst = (sign * num);
	return (1);
}

int	set_shlvl(t_msh *msh)
{
	char		*var;
	int			shlvl;
	static char	var_str[10] = "SHLVL=\0\0\0\0";


	var = get_env_val(msh->env, "SHLVL");
	if (!var || !*var || !is_numeric(var, &shlvl))
		return (builtin_export(msh, (char *[]){"export", "SHLVL=1", NULL}));
	if (shlvl < 0)
		return (builtin_export(msh, (char *[]){"export", "SHLVL=0", NULL}));
	if (shlvl > 998)
	{
		ft_dprintf(STDERR_FILENO, "minishell: warning: shell level (%d) too high, resetting to 1\n", shlvl);
		return (builtin_export(msh, (char *[]){"export", "SHLVL=1", NULL}));
	}
	ft_itoa2(shlvl + 1, var_str + 6);
	return (builtin_export(msh, (char *[]){"export", var_str, NULL}));
}
