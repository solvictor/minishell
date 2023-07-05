/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 08:54:00 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 14:50:06 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

void	child_process(t_msh *msh, t_cmd *cmd)
{
	g_context.n = CONT_CHILD_FORK;
	if (redirect_io(&msh->cmdline, cmd) == -1)
	{
		ft_dprintf(2, "Failed to redirect io\n");
		// not sure i can remove that
			//clear_cmdline(cmdline);
			//destroy_tokenlist(tokens);
		msh_terminate(msh, 1); // the fuck do i do here?
	}
	if (execve(cmd->path, cmd->args, msh->cmdline.envp) == -1)
	{
		ft_dprintf(STDOUT_FILENO, "minishell: %s: %s\n", cmd->path, strerror(errno));
			//clear_cmdline(cmdline);
			//destroy_tokenlist(tokens);
		msh_terminate(msh, 126);
	}
}

int	wait_pipeline(pid_t last_pid, int n_children)
{
	int	ret;
	int	stat_loc;

	ret = 0;
	while (n_children--)
	{
		if (wait(&stat_loc) == last_pid)
		{
			if (!WIFEXITED(stat_loc))
				ret = stat_loc;
			else
				ret = WEXITSTATUS(stat_loc);
		}
	}
	return (ret);
}
