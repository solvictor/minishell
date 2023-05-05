/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 02:08:08 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:19:46 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
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
		if (nb > (LONG_MAX / 10))
			return (-1 * (sign == 1));
		nb = (nb * 10) + (*nptr++ - 48);
		if (nb < 0)
			return (-1 * (sign == 1));
	}
	return (sign * (int)nb);
}
