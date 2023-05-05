/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 01:23:58 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:26:43 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*og_dest;

	if ((dest == src) || (n == 0))
		return (dest);
	og_dest = dest;
	if (dest < src)
		while (n--)
			*((unsigned char *)dest++) = *((unsigned char *)src++);
	else
		while (n--)
			*((unsigned char *)(dest + n)) = *((unsigned char *)(src + n));
	return (og_dest);
}
