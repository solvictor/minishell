/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handler_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 14:30:18 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/19 11:08:47 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// populates x with hexadecimal digits
void	get_hex_str(t_print *print, char *x, size_t n, int caps)
{
	size_t	pow;
	int		len;

	pow = 1;
	len = 1;
	while ((n / pow) >= 16)
	{
		pow *= 16;
		++len;
	}
	len = 0;
	while (pow > 0)
	{
		x[len++] = SET_HEX[(n / pow) % 16];
		if (caps && ft_isalpha(x[len - 1]))
				x[len - 1] -= 32;
		pow /= 16;
	}
	x[len] = '\0';
	print->gxl = len;
}

// populates x with decimal digits
void	get_diu_str(t_print *print, char *x, long n)
{
	long	pow;
	long	len;

	if (n < 0)
		print->pref = PREF_MINUS;
	n *= (1 - (n < 0) * 2);
	pow = 1;
	len = 1;
	while ((n / pow) >= 10)
	{
		pow *= 10;
		++len;
	}
	len = 0;
	while (pow > 0)
	{
		x[len++] = '0' + ((n / pow) % 10);
		pow /= 10;
	}
	x[len] = '\0';
	print->gxl = len;
}
