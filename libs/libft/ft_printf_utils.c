/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 16:34:53 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/19 11:09:00 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// initializes print struct with all proper values
// if PRINT_INIT is given it will completely reset print
// otherwise is will just set specific values to 0 for the next conversion
void	set_state(t_print *print, int mode, const char *s, int fd)
{
	if (mode == PRINT_INIT)
	{
		print->s = (char *)s;
		print->fd = fd;
		print->ret = 0;
		print->buf[0] = '\0';
		print->pos = 0;
	}
	print->convi = 0;
	print->convc = '\0';
	print->gxl = 0;
	print->fillc = ' ';
	print->flags = 0;
	print->width = 0;
	print->preci = -1;
	print->pref = STR_ZERO;
	print->handler = NULL;
	print->bad = 0;
}

// outputs the buffer to the fd set in print struct and resets the inner
// write position to 0
int	output_full(t_print *print)
{
	if (print->pos == PRINT_SIZE)
	{
		if (write(print->fd, print->buf, print->pos) == -1)
			return (-1);
		print->pos = 0;
	}
	return (0);
}

// gradually fills the buffer with strings passed from the handler functions
// and once full writes to the fd
int	fill_buf(t_print *print, char *s, int n, void *freeable)
{
	int	len;

	len = 0;
	while ((n < 0 && s[len]) || (n > 0 && len < n))
	{
		if (n == -2 && s[len] == '%')
			break ;
		print->buf[print->pos++] = s[len++];
		if (output_full(print) == -1)
			return (free(freeable), -1);
	}
	print->ret += len;
	if (s == print->s)
		print->s += len;
	return (free(freeable), 0);
}
