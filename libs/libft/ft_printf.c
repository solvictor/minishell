/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 19:25:46 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/25 15:50:18 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

// outputs format with processed conversions from va_list
int	ft_printf(const char *s, ...)
{
	va_list	valist;
	int		ret;

	va_start(valist, s);
	ret = ft_vdprintf(PRINT_FD, s, valist);
	va_end(valist);
	return (ret);
}

// same as printf but takes a file descriptor and a va_list as arguments
int	ft_vdprintf(int fd, const char *s, va_list valist)
{
	t_print	print;

	set_state(&print, PRINT_INIT, s, fd);
	while (*(print.s))
	{
		if (*(print.s) == '%')
		{
			if (set_conv_state(&print) == -1)
				return (write(fd, print.buf, print.pos), -1);
			else if (print.handler(&print, valist) == -1)
				return (-1);
		}
		else if (fill_buf(&print, print.s, -2, 0) == -1)
			return (-1);
	}
	if (write(fd, print.buf, print.pos) == -1)
		return (-1);
	return (print.ret);
}

int	ft_dprintf(int fd, const char *s, ...)
{
	va_list	valist;
	int		ret;

	va_start(valist, s);
	ret = ft_vdprintf(fd, s, valist);
	va_end(valist);
	return (ret);
}
