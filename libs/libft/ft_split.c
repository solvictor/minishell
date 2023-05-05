/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 22:12:26 by nlegrand          #+#    #+#             */
/*   Updated: 2023/02/06 13:07:55 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	i;

	while (*s && *s == c)
		++s;
	i = 0;
	while (*s)
	{
		if (*s != c)
			++i;
		while (*s && *s != c)
			++s;
		while (*s && *s == c)
			++s;
	}
	return (i);
}

static size_t	strlen_word(const char *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		++len;
	return (len);
}

static int	cpy_words(char **dest, const char *s, char c)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			len = strlen_word(s, c);
			dest[i] = (char *)malloc(sizeof(char) * (len + 1));
			if (dest[i] == NULL)
			{
				while (i--)
					free(dest[i]);
				return (-1);
			}
			ft_strlcpy(dest[i++], s, len + 1);
			s += len;
		}
		while (*s && *s == c)
			++s;
	}
	return (0);
}

char	**ft_split(const char *s, char c)
{
	char	**tmp;
	size_t	i;

	if (s == NULL)
		return (NULL);
	i = count_words(s, c);
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	if (tmp == NULL)
		return (NULL);
	if (cpy_words(tmp, s, c) == -1)
		return (free(tmp), NULL);
	tmp[i] = NULL;
	return (tmp);
}
