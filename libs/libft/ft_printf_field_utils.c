/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_field_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@stud.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:00:28 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/19 11:07:33 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// disables overridden flags and sets prefix
void	set_compat(t_print *print)
{
	const char	c = print->convc;

	if (print->flags & FM_PLUS)
		print->flags &= ~((unsigned int)FM_SPACE);
	if (print->flags & FM_MINUS || print->preci != -1)
		print->flags &= ~((unsigned int)FM_ZERO);
	if (c == 'p')
		print->pref = PREF_XL;
	if (c == 'x' && print->flags & FM_HASH)
		print->pref = PREF_XL;
	else if (c == 'X' && print->flags & FM_HASH)
		print->pref = PREF_XU;
	else if (c == 'd' || c == 'i')
	{
		if (print->pref[0] != '-' && (print->flags & FM_PLUS))
			print->pref = PREF_PLUS;
		else if (print->pref[0] != '-' && (print->flags & FM_SPACE))
			print->pref = PREF_SPACE;
	}
	if ((print->flags & FM_ZERO)
		|| print->width <= print->preci)
		print->fillc = '0';
}

// populates field with the string
int	set_field_str(t_print *print, char *field, char *x, int max)
{
	int	len;

	len = print->width - max;
	len *= ((print->flags & FM_MINUS) == 0);
	ft_memcpy(field + len, x, max);
	return (fill_buf(print, field, print->width, field));
}

// populates field with the numerical string
int	set_field_nums(t_print *print, char *field, char *x, int max)
{
	const int	f = print->flags;
	int			len;
	int			pdiff;

	pdiff = print->preci - ft_strlen(x);
	pdiff *= (pdiff >= 0);
	if (x[0] == '0' && print->preci == 0)
	{
		ft_memcpy(field + ((f & FM_MINUS) == 0) * (max - 1), print->pref,
			ft_strlen(print->pref));
		return (fill_buf(print, field, -1, field));
	}
	len = max - comb_len(x, print->pref) - pdiff;
	len *= ((f & FM_MINUS) == 0);
	ft_memcpy(field + len * ((f & FM_ZERO) == 0),
		print->pref, ft_strlen(print->pref));
	len += ft_strlen(print->pref);
	ft_memset(field + len, '0', pdiff);
	len += pdiff;
	ft_memcpy(field + len, x, ft_strlen(x));
	return (fill_buf(print, field, -1, field));
}

void	set_field_bad(t_print *print, char *field, int *len)
{
	const char	f = print->flags;

	if (f & FM_HASH)
		field[(*len)++] = '#';
	if (f & FM_PLUS)
		field[(*len)++] = '+';
	else if (f & FM_SPACE)
		field[(*len)++] = ' ';
	if (f & FM_MINUS)
		field[(*len)++] = '-';
	else if (f & FM_ZERO)
		field[(*len)++] = '0';
}
