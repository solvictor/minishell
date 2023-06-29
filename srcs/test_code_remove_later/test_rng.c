/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rng.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 22:48:56 by nlegrand          #+#    #+#             */
/*   Updated: 2023/06/29 13:22:09 by vegret           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_heredoc_name(char *filename, unsigned int rand)
{
	int	offset;
	int	i;

	offset = 12;
	i = 10;
	while (rand != 0)
	{
		filename[offset + i--] = '0' + rand % 10;
		rand /= 10;
	}
	while (i > 0)
		filename[offset + i--] = '0';
}

char	*random_name(t_msh *msh)
{
	int			i;
	t_rng		*rng;
	static char	heredoc_name[] = ".msh_heredoc_0000000000\0";

	rng = &msh->rng;
	i = 0;
	while (i < 1024)
	{
		rng->rand = rng_bit_rot((rng->rand + rng->inc) * rng->mult);
		set_heredoc_name(heredoc_name, rng->rand);
		if (access(heredoc_name, F_OK) != 0)
			return (heredoc_name);
		++i;
	}
	return (NULL);
	// REMOVE COMMENTS AFTER THIS FUNCTION, NORMINETTE DOESN'T CATCH THIS ERROR
}

	//printf("fd_urandom --> %d\n", rng->fd_urandom);
	//printf("curr_rand ---> %zd\n", rng->curr_rand);
	//printf("multiplier --> %zd\n", rng->multiplier);
	//printf("increment ---> %zd\n", rng->increment);

	//printf("%u\n", (unsigned int)-1);
	//set_heredoc_name(heredoc_name, 1);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 0);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 9);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 10);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 11);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 100);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 101);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 99);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 4294967295);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, (unsigned int)4294967296);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, (unsigned int)4294967297);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2147483647);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2147483648);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 2147483649);
	//printf("%s\n", heredoc_name);
	//set_heredoc_name(heredoc_name, 69201337);
	//printf("%s\n", heredoc_name);

int	create_heredoc(t_msh *msh, char *delimiter)
{
	const char	*name = random_name(msh);
	const int	size_delim = ft_strlen(delimiter) + 1;
	char		*line;
	int			fd;

	if (!name)
		return (1);
	fd = open(name, O_CREAT | O_RDWR, 0644);
	if (fd == -1)
		return (2);
	while (1)
	{
		line = readline(NULL);
		if (ft_strncmp(line, delimiter, size_delim) == 0)
			break ;
		if (write(fd, line, ft_strlen(line)) < 0)
			return (3);
	}
	return (0);
}

int	test_heredoc(t_msh *msh, char *delimiter)
{
	const pid_t	ret = fork();
	int			heredoc_fd;

	heredoc_fd = create_heredoc(msh, delimiter);
	if (ret == -1)
		return (-1);
	if (ret == 0)
	{
		if (dup2(STDIN_FILENO, heredoc_fd) == -1)
			return (-1);
		execve("/bin/cat", (char *[]) {"cat", NULL}, env_to_arr(msh->env));
		return (0);
	}
	if (close(heredoc_fd) == -1)
		return (-1);
	waitpid(ret, NULL, 0);
	return (0);
}
