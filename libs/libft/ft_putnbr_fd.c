/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 18:22:10 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/19 10:27:43 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nn;
	int		neg;
	int		len;
	char	tmp[11];
	char	*ptr;

	nn = (long)n * (1 - (neg = n < 0) * 2);
	tmp[0] = '-';
	len = neg;
	while (nn >= 10)
	{
		tmp[len++] = nn % 10 + 48;
		nn /= 10;
	}
	tmp[len++] = nn % 10 + 48;
	ptr = tmp + len - 1;
	while ((tmp + neg) < ptr)
	{
		nn = tmp[neg];
		tmp[neg++] = *ptr;
		*ptr-- = (char)nn;
	}
	write(fd, tmp, len);
}
