/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 17:38:09 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:33:37 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*tmp;
	size_t	i;
	size_t	size;

	if (s == NULL)
		return (NULL);
	if ((size_t)start >= ft_strlen(s) || len == 0)
		size = 1;
	else if (ft_strlen(s + start) > len)
		size = len + 1;
	else
		size = ft_strlen(s + start) + 1;
	tmp = (char *)malloc(sizeof(char) * size);
	if (tmp == NULL)
		return (NULL);
	i = 0;
	while (i < (size - 1) && s[(size_t)start + i])
	{
		tmp[i] = s[(size_t)start + i];
		++i;
	}
	tmp[i] = '\0';
	return (tmp);
}
