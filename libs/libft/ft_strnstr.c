/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 22:51:42 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:54:10 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hay, const char *nee, size_t len)
{
	size_t	i;
	size_t	j;

	if ((hay == NULL) && (len == 0))
		return (NULL);
	if (nee[0] == '\0')
		return ((char *)hay);
	i = 0;
	while (*hay && (i < len))
	{
		j = 0;
		while ((hay[j] == nee[j]) && hay[j] && ((i + j) < len))
		{
			if (nee[j + 1] == 0)
				return ((char *)hay);
			++j;
		}
		++hay;
		++i;
	}
	return (NULL);
}
