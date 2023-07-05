/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 09:56:54 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 16:40:50 by nlegrand         ###   ########.fr       */
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

static	int	heredoc_loop(t_msh *msh, const char *limiter, int fd)
{
	const int	size_limiter = ft_strlen(limiter) + 1;
	char		*line;
	int			count;

	count = 1;
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (ft_dprintf(STDERR_FILENO,
				"minishell: warning: here-document at line %d delimited \
by end-of-file (wanted `%s')\n", count, limiter), 0);
		if (ft_strncmp(line, limiter, size_limiter) == 0)
			return (free(line), 0);
		if (expand_str(msh, &line) == -1)
			return (free(line), -1);
		if (write(fd, line, ft_strlen(line)) < 0)
			return (free(line), -1);
		if (write(fd, "\n", 1) < 0)
			return (free(line), -1);
		free(line);
		++count;
	}
	return (0);
}

static int	read_heredoc(t_msh *msh, const char *name, const char *limiter)
{
	int	fd;
	int	ret;

	fd = open(name, O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		return (-1);
	g_context.heredoc_fd = fd;
	ret = heredoc_loop(msh, limiter, fd);
	return (close(fd), ret);
}

static int	make_heredoc(t_msh *msh, t_tokenlist *token)
{
	pid_t		pid;
	static char	name[] = ".msh_heredoc_0000000000\0";
	int			stat_loc;

	set_heredoc_name(name, token->data_opt = randuint_rng(&msh->rng));
	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
	{
		g_context.n = CONT_HEREDOC;
		if (read_heredoc(msh, name, token->data) == -1)
		{
			msh_terminate(msh, EXIT_FAILURE);
		}
		msh_terminate(msh, EXIT_SUCCESS);
	}
	wait(&stat_loc); // ret??? need to protect for good error return
	if (!WIFEXITED(stat_loc))
		return (-1);
	return (0);
}

int	do_heredocs(t_msh *msh)
{
	t_tokenlist	*cur;

	cur = msh->tokens;
	while (cur)
	{
		if (cur->type == HEREDOC && make_heredoc(msh, cur) == -1)
			return (unlink_heredocs(msh->tokens), -1);
		cur = cur->next;
	}
	return (0);
}
