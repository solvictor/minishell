/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_better.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@stud.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 17:14:50 by nlegrand          #+#    #+#             */
/*   Updated: 2022/12/29 17:29:03 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Doesn't accomplish error detection at all like I wanted to do first
// Instead I just made a neat little wrapper to use get_next_line in while
// loops
int	gnl_w(int fd, char **line)
{
	*line = get_next_line(fd);
	if (*line == NULL)
		return (-1);
	return (0);
}
