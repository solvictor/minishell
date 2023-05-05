/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 00:38:03 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:56:40 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	void	*og_dest;

	if ((dest == NULL) && (src == NULL))
		return (NULL);
	og_dest = dest;
	while (n > sizeof(size_t))
	{
		*(size_t *)dest = *(size_t *)src;
		dest += sizeof(size_t);
		src += sizeof(size_t);
		n -= sizeof(size_t);
	}
	while (n--)
		*(unsigned char *)dest++ = *(unsigned char *)src++;
	return (og_dest);
}
