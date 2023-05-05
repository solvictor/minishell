/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_conv_parser.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@stud.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 22:26:17 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/19 11:07:23 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// sets print struct options with proper value by parsing parts of the format
// string with subfunctions
int	set_conv_state(t_print *print)
{
	char	*s;

	set_state(print, PRINT_RESET, NULL, -1);
	s = print->s + 1;
	if (get_flags(print, &s) == -1)
		return (-1);
	if (get_width(print, &s) == -1)
		return (-1);
	if (get_preci(print, &s) == -1)
		return (-1);
	if (get_conv(print, &s) == -1)
		return (-1);
	print->s = s;
	return (0);
}

// parse flags from format
int	get_flags(t_print *print, char **s)
{
	char	*ss;
	char	*f;
	int		mask;

	ss = *s;
	while (*ss)
	{
		f = ft_strchr(SET_FLAG, *ss);
		if (f == NULL)
			break ;
		mask = 1;
		mask <<= f - SET_FLAG;
		print->flags |= mask;
		++ss;
	}
	if (*ss == '\0')
		return (-1);
	*s = ss;
	return (0);
}

// parse width from format
int	get_width(t_print *print, char **s)
{
	char	*ss;

	ss = *s;
	print->width = atoi_safe(ss);
	if (print->width == -1)
		return (-1);
	while (ft_isdigit(*ss))
		++ss;
	if (*ss == '\0')
		return (-1);
	*s = ss;
	return (0);
}

// parse precision from format
int	get_preci(t_print *print, char **s)
{
	char	*ss;

	ss = *s;
	if (*ss != '.')
		return (0);
	++ss;
	print->preci = atoi_safe(ss);
	while (ft_isdigit(*ss))
		++ss;
	if (print->preci == -1 || *ss == 0)
		return (-1);
	*s = ss;
	return (0);
}

// parse conversion from format
int	get_conv(t_print *print, char **s)
{
	char		*ss;
	char		*c;
	int			mask;
	static int	(*handlers[])(t_print *print, va_list valist)
		= {hdl_c, hdl_s, hdl_ps, hdl_p, hdl_di, hdl_di, hdl_u, hdl_x, hdl_x};

	ss = *s;
	if (*ss == '\0')
		return (-1);
	c = ft_strchr(SET_CONV, *ss);
	if (c == NULL)
	{
		print->bad = 1;
		print->handler = hdl_bad;
		*s = ss;
		return (0);
	}
	mask = 1;
	mask <<= c - SET_CONV;
	print->convi = mask;
	print->convc = *c;
	print->handler = handlers[c - SET_CONV];
	*s = ss + 1;
	return (0);
}
