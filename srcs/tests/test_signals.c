/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 16:33:20 by vegret            #+#    #+#             */
/*   Updated: 2023/06/19 19:18:26 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", ft_strlen(MSH_PROMPT));
		rl_redisplay();
	}
}

int	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handler;
	sa.sa_flags = SA_RESTART;
	if (sigemptyset(&sa.sa_mask) != 0)
		return (-1);
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (-1);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (-1);
	return (0);
}
