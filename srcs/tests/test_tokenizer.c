/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 16:25:37 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/26 16:09:11 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns 1 if one given character is a bash whitespace
static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// Returns 1 if one given character is a bash metacharacter
static int	is_metachar(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')' || c == '<' ||
		c == '>');
}

static int	get_str_quoted(const char *str, char **dst)
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
	return (len + 2); // could be useful for storing length of token in struct later // added +2 check if ok
}
static int	get_str_unquoted(const char *str, char **dst)
{
	int			len;
	int			i;

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
	return (len); // could be useful for storing length of token in struct later
}

static	t_tokenlist	*token_add_front(t_tokenlist **begin, char *data)
{
	t_tokenlist	*tmp;

	tmp = malloc(sizeof(t_tokenlist));
	if (tmp == NULL)
		return (NULL);
	tmp->data = data;
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}

void	destroy_tokenlist(t_tokenlist **begin)
{
	t_tokenlist *tmp;

	while (*begin)
	{
		tmp = *begin;
		*begin = (*begin)->next;
		free(tmp->data);
		free(tmp);
	}
}

static void	reverse_tokens(t_tokenlist **begin)
{
	t_tokenlist *curr;
	t_tokenlist *prev;
	t_tokenlist *next;

	curr = *begin;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*begin = prev;
}

//static int	strlen_quoted(const char *str)
//{
//
//	const char	limiter = *str;
//	int			len;
//
//	++str;
//	len = 0;
//	while (str[len] && str[len] != limiter)
//		++len;
//	if (str[len] != limiter)
//		return (-1);
//	return (len);
//}
//static int	strlen_unquoted(const char *str)
//{
//	int	len;
//
//	len = 0;
//	while (str[len] && (!is_whitespace(str[len]) && !is_metachar(str[len])
//		&& str[len] != '\'' && str[len] != '"'))
//		++len;
//	return (len);
//}

//static int	copy_str(const char *str, char *dst)
//{
//	int	i;
//	int	len;
//	int	ret;
//
//	i = 0;
//	len = 0;
//	while (str[i] && !is_metachar(str[i]) && !is_whitespace(str[i]))
//	{
//		if (str[i] == '\'' || str[i] == '"')
//		{
//			ret = strlen_quoted(str + i);
//			ft_memcpy(dst + len, str + i + 1, ret);
//			len += ret;
//			i += ret + 2;
//		}
//		else
//		{
//			ret = strlen_unquoted(str + i);
//			ft_memcpy(dst + len, str + i, ret);
//			len += ret;
//			i += ret;
//		}
//	}
//	dst[len] = '\0';
//	return (i);
//}

//static int	get_str(const char *str, char **dst)
//{
//	int	len;
//
//	len = get_strlen(str);
//	if (len == -1)
//		return (-2);
//	*dst = malloc(sizeof(char) * (len + 1));
//	if (*dst == NULL)
//		return (-1);
//	return (copy_str(str, *dst));
//}

static t_tokentype	get_metachar_tokentype(const char *input, int *i)
{
	if (input[*i] == '(')
		return (++(*i), L_BRACKET);
	else if (input[*i] == ')')
		return (++(*i), R_BRACKET);
	else if (input[*i] == '|' && input[*i + 1] != '|')
		return (++(*i), PIPE);
	else if (input[*i] == '|' && input[*i + 1] == '|')
		return (((*i) += 2), LOGIC_OR);
	else if (input[*i] == '&' && input[*i + 1] == '&')
		return (((*i) += 2), LOGIC_AND);
	else if (input[*i] == '<' && input[*i + 1] != '<')
		return (++(*i), L_REDIR_TRUNC);
	else if (input[*i] == '>' && input[*i + 1] != '>')
		return (++(*i), R_REDIR_TRUNC);
	else if (input[*i] == '<' && input[*i + 1] == '<')
		return (((*i) += 2), L_REDIR_APPEND);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		return (((*i) += 2), R_REDIR_APPEND);
	else
		return (++(*i), UNKNOWN); // what the fuck do i do in this case??
}

static int	push_metachar_token(t_tokenlist **tokens, const char *input, int *i)
{
	if (token_add_front(tokens, NULL) == NULL)
		return (printf("malloc error token_add_front\n"), -1);
	(*tokens)->type = get_metachar_tokentype(input, i);
	(*tokens)->concat_next = 0;
	return (0);
}

static char	*get_str_token(const char *input, int *i)
{
	int		ret;
	char	*tmp;

	if (input[*i] == '"' || input[*i] == '\'')
		ret = get_str_quoted(input + *i, &tmp);
	else
		ret = get_str_unquoted(input + *i, &tmp);
	if (ret == -1)
		return (printf("malloc error in get_str_token\n"), NULL);
	else if (ret == -2)
		return (printf("Unmatch quotation\n"), NULL);
	*i += ret;
	return (tmp);
}

static int	push_str_token(t_tokenlist **tokens, const char *input, int *i)
{
	if (token_add_front(tokens, NULL) == NULL)
		return (printf("malloc error token_add_front\n"), -1);
	if (input[*i] == '"')
		(*tokens)->type = DOUBLE_QUOTED_STR;
	else if (input[*i] == '\'')
		(*tokens)->type = SINGLE_QUOTED_STR;
	else
		(*tokens)->type = UNQUOTED_STR;
	(*tokens)->data = get_str_token(input, i);
	if ((*tokens)->data == NULL)
		return (printf("malloc error get_str_token\n"), -1);
	if (input[*i] && !is_metachar(input[*i]) && !is_whitespace(input[*i]))
		(*tokens)->concat_next = 1;
	else
		(*tokens)->concat_next = 0;
	return (0);
}

int	tokenize(t_msh *msh, const char *input, t_tokenlist **tokens)
{
	int			i;

	(void)msh;
	*tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			++i;
		if (input[i] && !is_metachar(input[i]))
		{
			if (push_str_token(tokens, input, &i) == -1)
				return (destroy_tokenlist(tokens), -1);
		}
		//{
		//	ret = get_str(input + i, &tmp);
		//	if (ret == -1)
		//		return (printf("malloc error get_str_quoted\n"), -1);
		//	else if (ret == -2)
		//		return (printf("unmatch quotation\n"), -1);
		//	if (token_add_front(tokens, tmp) == NULL)
		//		return (free(tmp), printf("malloc error token_add_front\n"),
		//			-1);
		//	(*tokens)->type = STR;
		//	i += ret;
		//}
		else if (input[i])
			if (push_metachar_token(tokens, input, &i) == -1)
				return (destroy_tokenlist(tokens), -1);
	}
	reverse_tokens(tokens);
	return (0);
}

static void	display_token_type(t_tokenlist *token)
{
	printf("token type -> ");
	if (token->type == UNQUOTED_STR)
		printf("UNQUOTED_STR\n");
	else if (token->type == DOUBLE_QUOTED_STR)
		printf("DOUBLE_QUOTED_STR\n");
	else if (token->type == SINGLE_QUOTED_STR)
		printf("SINGLE_QUOTED_STR\n");
	else if (token->type == PIPE)
		printf("PIPE\n");
	else if (token->type == LOGIC_OR)
		printf("LOGIC_OR\n");
	else if (token->type == LOGIC_AND)
		printf("LOGIC_AND\n");
	else if (token->type == L_REDIR_TRUNC)
		printf("L_REDIR_TRUNC\n");
	else if (token->type == R_REDIR_TRUNC)
		printf("R_REDIR_TRUNC\n");
	else if (token->type == L_REDIR_APPEND)
		printf("L_REDIR_APPEND\n");
	else if (token->type == R_REDIR_APPEND)
		printf("R_REDIR_APPEND\n");
	else if (token->type == L_BRACKET)
		printf("L_BRACKET\n");
	else if (token->type == R_BRACKET)
		printf("R_BRACKET\n");
	else
		printf("UNKNOWN\n");
}

void	display_tokens(t_tokenlist *begin)
{
	t_tokenlist	*curr;
	int			i;

	curr = begin;
	i = 0;
	while (curr)
	{
		printf("TOKEN #%d\n", i);
		printf("token data -> %s", curr->data);
		if (curr->concat_next)
			printf(" --->");
		printf("\n");
		display_token_type(curr);
		printf("\n");
		curr = curr->next;
		++i;
	}
}

void	test_tokenizer(t_msh *msh)
{
	int		ret;
	t_tokenlist	*tokens; // can be made into an ArrayList

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return ((void)printf(MSH_ERROR ME_TOKEN_CMD));
	display_tokens(tokens);
	destroy_tokenlist(&tokens);
}
