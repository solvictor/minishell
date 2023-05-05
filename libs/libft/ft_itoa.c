/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 16:06:40 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:24:00 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	long	nn;
	long	pow;
	int		len_pow;
	char	*tmp;

	nn = (long)n * (1 + (n < 0) * -2);
	pow = 1;
	len_pow = 1 + (n < 0);
	while ((nn / pow) >= 10)
	{
		pow *= 10;
		++len_pow;
	}
	tmp = (char *)malloc(sizeof(char) * (len_pow + 1));
	if (tmp == NULL)
		return (NULL);
	tmp[0] = '-';
	len_pow = (n < 0);
	while (pow > 0)
	{
		tmp[len_pow++] = '0' + ((nn / pow) % 10);
		pow /= 10;
	}
	tmp[len_pow] = '\0';
	return (tmp);
}
