/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:52:41 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/05 16:58:24 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns 1 if one given character is a bash whitespace
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// Returns 1 if one given character is a bash metacharacter
int	is_metachar(char c)
{
	return (c == '|' || c == '(' || c == ')' || c == '<'
		|| c == '>');
}

// Looks at the character of index 'i' in the input and determines the tokentype
// 'i' will be modified for the processing of the next input
t_tokentype	get_metachar_tokentype(const char *input, int *i)
{
	if (input[*i] == '|')
		return (++(*i), PIPE);
	else if (input[*i] == '<' && input[*i + 1] != '<')
		return (++(*i), I_FILE);
	else if (input[*i] == '>' && input[*i + 1] != '>')
		return (++(*i), O_FILE_TRUNC);
	else if (input[*i] == '<' && input[*i + 1] == '<')
		return (((*i) += 2), HEREDOC);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		return (((*i) += 2), O_FILE_APPEND);
	else
		return (++(*i), UNKNOWN);
}

// Stores a quoted string portion of the input into dst
// return 0 on success, -2 if the quotation is not closed, -1 for other errors
int	get_str_quoted(const char *str, char **dst)
{
	const char	limiter = *str;
	int			len;
	int			i;

	*dst = NULL;
	++str;
	len = 0;
	while (str[len] && str[len] != limiter)
		++len;
	if (str[len] != limiter)
		return (-2);
	*dst = malloc(sizeof(char) * (len + 1));
	if (*dst == NULL)
		return (-1);
	i = 0;
	while (i < len)
	{
		(*dst)[i] = str[i];
		++i;
	}
	(*dst)[i] = '\0';
	return (len + 2);
}

// Stores a string delimited by metacharacters from the input into dst
// Returns 0 on success, -1 otherwise
int	get_str_unquoted(const char *str, char **dst)
{
	int	len;
	int	i;

	*dst = NULL;
	len = 0;
	while (str[len] && (!is_whitespace(str[len]) && !is_metachar(str[len])
			&& str[len] != '\'' && str[len] != '"'))
		++len;
	*dst = malloc(sizeof(char) * (len + 1));
	if (*dst == NULL)
		return (-1);
	i = 0;
	while (i < len)
	{
		(*dst)[i] = str[i];
		++i;
	}
	(*dst)[i] = '\0';
	return (len);
}
