/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 09:56:54 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/04 20:30:19 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_context	g_context;

char	*set_heredoc_name(char *filename, long rand)
{
	int				offset;
	int				i;
	unsigned int	urand;


	urand = rand;
	offset = 12;
	i = 10;
	while (urand != 0)
	{
		filename[offset + i--] = '0' + urand % 10;
		urand /= 10;
	}
	while (i > 0)
		filename[offset + i--] = '0';
	return (filename);
}

int	read_heredoc(t_msh *msh, const char *name, const char *limiter) // JUST COPIED FROM TEST_RNG, NOT VERIFIED AT ALL
{
	const int	size_limiter = ft_strlen(limiter) + 1;
	char		*line;
	int			fd;

	// removed name check here, pray it never happens (should be protected already tho)
	//if (!name)
	//	return (-1);
	fd = open(name, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		return (-1);
	g_context.heredoc_fd = fd;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, limiter, size_limiter) == 0)
			break ;
		if (expand_str(msh, &line) == -1)
			return (free(line), close(fd), -1);
		if (write(fd, line, ft_strlen(line)) < 0)
			return (free(line), close(fd), -1);
		if (write(fd, "\n", 1) < 0)
			return (free(line), close(fd), -1);
		free(line);
	}
	return (close(fd), 0);
}

static int	make_heredoc(t_msh *msh, t_tokenlist *token)
{
	pid_t		pid;
	static char	name[] = ".msh_heredoc_0000000000\0";

	set_heredoc_name(name, token->data_opt = randuint_rng(&msh->rng));
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		g_context.n = CONT_HEREDOC;
		if (read_heredoc(msh, name, token->data) == -1)
		{
			msh_terminate(msh);
			exit(EXIT_FAILURE); // not sure i should make it fail here
		}
		msh_terminate(msh);
		exit(EXIT_SUCCESS);
	}
	wait(NULL); // ret??? need to protect for good error return
	return (0);
	//(void)msh;
	//(void)cmdline;
	//(void)token;
	//return (0);
}

int	do_heredocs(t_msh *msh, t_cmdline *cmdline)
{
	t_tokenlist	*cur;

	cur =  cmdline->cmds[0].start_token;
	while (cur)
	{
		if (cur->type == HEREDOC && make_heredoc(msh, cur) == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}
