/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:52:11 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/09 14:39:07 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	dup;
	size_t	i;
	void	*og_s;

	dup = 0;
	og_s = s;
	i = 0;
	while ((n > sizeof(size_t)) && (i++ < sizeof(size_t)))
	{
		dup <<= 8;
		dup |= (unsigned char)c;
	}
	while (n > sizeof(size_t))
	{
		*(size_t *)s = dup;
		s += sizeof(size_t);
		n -= sizeof(size_t);
	}
	while (n--)
		*(unsigned char *)s++ = (unsigned char)c;
	return (og_s);
}
