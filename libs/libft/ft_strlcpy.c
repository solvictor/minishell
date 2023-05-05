/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:18:11 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:31:14 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len_src;

	len_src = 0;
	while (src[len_src])
		++len_src;
	if (size == 0)
		return (len_src);
	while (*src && (size-- > 1))
		*dst++ = *src++;
	*dst = '\0';
	return (len_src);
}
