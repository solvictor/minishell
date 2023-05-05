/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 18:36:22 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/11 14:44:34 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// makes on read in a newly allocated struct
t_line	*read_one(int fd, int *ret, int *len, char *buf)
{
	t_line	*tmp;
	char	*end;

	tmp = malloc(sizeof(t_line));
	if (tmp == NULL)
		return (NULL);
	*ret = 0;
	if (buf && *buf)
		while (*buf)
			tmp->buf[(*ret)++] = *buf++;
	else
		*ret = read(fd, tmp->buf, BUFFER_SIZE);
	if (*ret == -1)
		return (free(tmp), NULL);
	tmp->buf[*ret] = '\0';
	end = get_endl(tmp->buf);
	if (end == NULL)
		*len += BUFFER_SIZE;
	else
		*len += end - tmp->buf + (*end == '\n');
	tmp->next = NULL;
	return (tmp);
}

// assembles the line segments into a single string
char	*make_line(t_line **line, int len)
{
	char	*tmp;
	t_line	*curr;
	int		i;
	int		j;

	tmp = malloc(sizeof(char) * (len + 1));
	if ((tmp == NULL) || (len == 0))
		return (free(tmp), clear_line(line), NULL);
	curr = *line;
	i = 0;
	while (curr != NULL)
	{
		j = 0;
		while (curr->buf[j] && (i < len))
			tmp[i++] = curr->buf[j++];
		if (i == len)
			break ;
		curr = curr->next;
	}
	tmp[len] = '\0';
	clear_line(line);
	return (tmp);
}

// trims tmpbuf after eol and saves the rest to static var buf
void	split_buf_rest(char *tmpbuf, char *buf)
{
	int		i;
	char	*end;

	i = 0;
	end = get_endl(tmpbuf);
	if (end == NULL)
		return ((void)(*buf = '\0'));
	while (end[i] != '\0')
	{
		buf[i] = end[i + 1];
		++i;
	}
	buf[i] = '\0';
	end[1] = '\0';
}

// frees the line chained list
void	clear_line(t_line **line)
{
	t_line	*curr;
	t_line	*tmp;

	if (line && *line)
	{
		curr = *line;
		while (curr != NULL)
		{
			tmp = curr;
			curr = curr->next;
			free(tmp);
		}
		*line = NULL;
	}
}

// returns pointer to end of line in buffer, NULL otherwise
char	*get_endl(char *buf)
{
	int	i;

	i = 0;
	if (buf == NULL)
		return (NULL);
	while (buf[i])
	{
		if (buf[i] == '\n')
			return (buf + i);
		++i;
	}
	if (i == BUFFER_SIZE)
		return (NULL);
	return (buf + i);
}
