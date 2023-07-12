/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 16:06:40 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/12 19:09:12 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa2(int n, char *dst)
{
	long	nn;
	long	pow;
	int		len_pow;

	nn = (long)n * (1 + (n < 0) * -2);
	pow = 1;
	len_pow = 1 + (n < 0);
	while ((nn / pow) >= 10)
	{
		pow *= 10;
		++len_pow;
	}
	dst[0] = '-';
	len_pow = (n < 0);
	while (pow > 0)
	{
		dst[len_pow++] = '0' + ((nn / pow) % 10);
		pow /= 10;
	}
	dst[len_pow] = '\0';
	return (dst);
}
