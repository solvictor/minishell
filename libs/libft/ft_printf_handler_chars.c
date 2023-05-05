/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handler_chars.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 22:43:10 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/19 11:07:46 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// appends char conversion to buffer
int	hdl_c(t_print *print, va_list valist)
{
	char	c;
	char	*field;

	if (malloc_safe(print, &field, print->width + 1) == -1)
		return (-1);
	c = va_arg(valist, int);
	if (print->width > 1)
	{
		ft_memset(field, ' ', print->width);
		if (print->flags & FM_MINUS)
			field[0] = c;
		else
			field[print->width - 1] = c;
		return (fill_buf(print, field, print->width, field));
	}
	else
		return (fill_buf(print, &c, 1, field));
}

// apprends string conversion to buffer
int	hdl_s(t_print *print, va_list valist)
{
	char		*x;
	char		*field;
	int			max;

	if (malloc_safe(print, &field, print->width + 1) == -1)
		return (-1);
	x = va_arg(valist, char *);
	if (x == NULL && (int)ft_strlen(STR_NULL) > print->preci
		&& print->preci != -1)
		x = STR_ZERO;
	else if (x == NULL)
		x = STR_NULL;
	print->gxl = ft_strlen(x);
	max = mini(print->preci, print->gxl);
	max = (max != -1) * max + (max == -1) * print->gxl;
	set_compat(print);
	ft_memset(field, ' ', print->width);
	field[print->width] = '\0';
	if (print->width > max)
		return (set_field_str(print, field, x, max));
	if (max == print->gxl)
		return (fill_buf(print, x, -1, field));
	else
		return (fill_buf(print, x, max, field));
}

// appends % conversion to buffer
int	hdl_ps(t_print *print, va_list valist)
{
	(void)valist;
	return (fill_buf(print, "%", 1, NULL));
}

// appends buffer with the proper format for a failed conversion
int	hdl_bad(t_print *print, va_list valist)
{
	char			x[28];
	int				len;

	(void)valist;
	len = 0;
	x[len++] = '%';
	if (print->flags > 0)
		set_field_bad(print, x, &len);
	if (print->width != 0)
		get_diu_str(print, x + len, print->width);
	len += print->gxl;
	if (print->preci != -1)
	{
		x[len++] = '.';
		get_diu_str(print, x + len, print->preci);
	}
	len += print->gxl;
	x[len] = '\0';
	return (fill_buf(print, x, -1, NULL));
}
