/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 15:28:50 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/09 11:03:05 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*tmp;
	size_t	i;

	if (size && nmemb > (ULONG_MAX / size))
		return (NULL);
	tmp = malloc(nmemb * size);
	i = 0;
	if (tmp != NULL)
	{
		tmp[0] = 0;
		while (i < (nmemb * size))
			tmp[i++] = 0;
	}
	return (tmp);
}
