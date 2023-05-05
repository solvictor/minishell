/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 00:59:22 by nlegrand          #+#    #+#             */
/*   Updated: 2022/11/08 14:33:15 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isset(const char c, const char *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*start;
	char	*end;
	char	*tmp;
	size_t	size;

	if ((s1 == NULL) || (set == NULL))
		return (NULL);
	while (isset(*s1, set))
		++s1;
	start = (char *)s1;
	while (*s1)
		++s1;
	--s1;
	while (isset(*s1, set))
		--s1;
	end = (char *)s1;
	(void)(((start <= end) && (size = end - start + 2)) || (size = 1));
	tmp = (char *)malloc(sizeof(char) * size);
	if (tmp == NULL)
		return (NULL);
	if (size == 1)
		*tmp = '\0';
	else
		ft_strlcpy(tmp, start, size);
	return (tmp);
}
