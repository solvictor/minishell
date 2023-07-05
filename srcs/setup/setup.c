/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 06:25:47 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 11:20:33 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Gives default values to msh struct variables
static void	msh_init_vars(t_msh *msh)
{
	msh->env = NULL;
	msh->tokens = NULL;
	msh->cmdline.cmds_n = 0;
	msh->cmdline.cmds = NULL;
	msh->cmdline.pipes = NULL;
	msh->cmdline.paths = NULL;
	msh->cmdline.envp = NULL;
	msh->exit = 0;
	msh->ret = EXIT_SUCCESS;
	msh->rng.rand = 0;
	msh->rng.mult = 0;
	msh->rng.inc = 0;
}

// Handles SIGINT and SIGQUIT signals so it doesn't stop the shell when pressing
// Ctrl+C or Ctrl+\ (backslash)
static int	setup_signals(void)
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

// Converts the envp from main to a linked list of t_env structs
// Returns 0 on success, -1 otherwise
static int	setup_env(t_msh *msh, char **envp)
{
	int		i;
	t_env	**curr;
	t_env	*prev;

	i = 0;
	curr = &(msh->env);
	prev = NULL;
	while (envp[i])
	{
		*curr = env_new(envp[i]);
		if (*curr == NULL)
			return (destroy_env_list(&msh->env), -1);
		(*curr)->prev = prev;
		prev = *curr;
		curr = &(*curr)->next;
		++i;
	}
	return (0);
}

// Opens /dev/urandom to generate pseudo-random numbers for the heredoc naming
static int	setup_rng(t_rng *rng)
{
	int	fd_urandom;
	fd_urandom = open("/dev/urandom", O_RDONLY);
	if (fd_urandom == -1)
		return (-1);
	rng->rand = get_randint(fd_urandom);
	if (rng->rand == 0)
		return (close(fd_urandom), -1);
	rng->mult = get_randint(fd_urandom);
	if (rng->mult == 0)
		return (close(fd_urandom), -1);
	rng->inc = get_randint(fd_urandom);
	if (rng->inc == 0)
		return (close(fd_urandom), -1);
	return (close(fd_urandom), 0);
}

// Gives default values to msh struct variables, sets up the environment and
// RNG
int	msh_setup(t_msh *msh, int ac, char **envp)
{
	if (ac != 1)
		return (printf(MSH_ERROR ME_AC), -1);
	msh_init_vars(msh);
	if (setup_signals() != 0)
		return (printf(MSH_ERROR ME_ENV), -1);
	if (setup_env(msh, envp) != 0)
		return (printf(MSH_ERROR ME_ENV), -1);
	if (setup_rng(&msh->rng) == -1)
		return (printf(MSH_ERROR ME_RNG), destroy_env_list(&msh->env), -1);
	return (0);
}
