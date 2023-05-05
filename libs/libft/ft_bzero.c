/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 00:51:41 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:20:17 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	while (n > sizeof(size_t))
	{
		*(size_t *)s = 0;
		s += sizeof(size_t);
		n -= sizeof(size_t);
	}
	while (n--)
		*(unsigned char *)s++ = '\0';
}
