/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_expansion_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 19:06:55 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/13 20:28:11 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns the length of str with whitespace trimmed of the ends
size_t	trimmed_len(char *str)
{
	char	*start;
	char	*end;

	start = str;
	while (*start && is_whitespace(*start))
		++start;
	if (*start == '\0')
		return (0);
	end = &str[ft_strlen(str) - 1];
	while (*end && is_whitespace(*end))
		--end;
	return (end - start + 1);
}
