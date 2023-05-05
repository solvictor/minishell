/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 18:13:53 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:30:59 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	og_size;
	size_t	dst_len;
	size_t	src_len;

	if ((dst == NULL) && (size == 0))
		return (0);
	og_size = size;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	while (*dst && size)
	{
		++dst;
		--size;
	}
	if (size == 0)
		return (og_size + src_len);
	while (*src && (size-- > 1))
		*dst++ = *src++;
	*dst = '\0';
	return (dst_len + src_len);
}
