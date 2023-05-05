/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@stud.42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:00:33 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/19 11:08:53 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// can't fucking use VLAs so here's a small function to help catch malloc fails
int	malloc_safe(t_print *print, char **ptr, size_t n)
{
	*ptr = malloc(sizeof(char) * n);
	if (*ptr == NULL)
		return (output_full(print), -1);
	return (0);
}

// atoi but it detects INT overflow, not LONG overflow like a fucking idiot
int	atoi_safe(const char *nptr)
{
	long	nb;
	int		sign;

	nb = 0;
	while (*nptr && (((*nptr >= 9) && (*nptr <= 13)) || (*nptr == ' ')))
		++nptr;
	sign = 1;
	if ((*nptr == '-') || (*nptr == '+'))
		if (*nptr++ == '-')
			sign = -1;
	while ((*nptr >= '0') && (*nptr <= '9'))
	{
		nb = (nb * 10) + (*nptr++ - 48);
		if (nb > INT_MAX && sign == 1)
			return (-1);
		if (nb > (long)INT_MAX + 1 && sign == -1)
			return (0);
	}
	return (sign * (int)nb);
}

// simple max function, named 'maxi' for compatibility concerns
int	maxi(int a, int b)
{
	if (a >= b)
		return (a);
	return (b);
}

// simple min function, named 'mini' for compatibility concerns
int	mini(int a, int b)
{
	if (a <= b)
		return (a);
	return (b);
}

// combines len of 2 strings, only purpose is to shorten combined
// ft_strlen calls
int	comb_len(char *a, char *b)
{
	return (ft_strlen(a) + ft_strlen(b));
}
