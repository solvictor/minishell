/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 16:38:09 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/11 20:28:52 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

// Resets the signal handlers and behaviors to default with SIG_DFL
void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}

// Signal handling function for sigint (ctrl+c)
void	handler_sigint(int sig)
{
	(void)sig;
	if (g_context.cur == CONT_PARENT)
	{
		g_context.msh->ret = 130;
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_context.cur == CONT_HEREDOC)
	{
		ft_printf("\n");
		rl_on_new_line();
		msh_terminate(g_context.msh, 130);
	}
}
