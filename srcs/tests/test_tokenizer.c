/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 16:25:37 by nlegrand          #+#    #+#             */
/*   Updated: 2023/04/22 17:21:07 by nlegrand         ###   ########.fr       */
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
	return (len); // could be useful for storing length of token in struct later
}
static int	get_str_unquoted(const char *str, char **dst)
{
	int			len;
	int			i;

	*dst = NULL;
	len = 0;
	while (str[len] && (!is_whitespace(str[len]) && !is_metachar(str[len])))
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

//static char	**transfer_tokens(t_tokenlist **begin)
//{
//	int				count;
//	char			**tmp;
//	t_tokenlist	*curr;
//
//	count = 0;
//	curr = *begin;
//	while (curr && ++count)
//		curr = curr->next;
//	tmp = malloc(sizeof(char *) * (count + 1));
//	if (tmp == NULL)
//		return (NULL);
//	tmp[count] = NULL;
//	while (*begin)
//	{
//		tmp[--count] = (*begin)->data;
//		curr = *begin;
//		*begin = (*begin)->next;
//		free(curr);
//	}
//	return (tmp);
//}
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

int	tokenize(t_msh *msh, const char *input, t_tokenlist **tokens)
{
	(void)msh;
	//t_tokenlist	*tokenlist; // can be replaced with t_list from libft and turned into an array of tokenlist later
	char		*tmp;
	int			i;
	int			len;

	*tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			++i;
		if (input[i] == '\'' || input[i] == '"')
		{
			// concat loop here maybe? // NO, can be done after with a struct and a flag to concat the next string // or maybe yes idk we'll see
			len = get_str_quoted(input + i, &tmp);
			if (len == -1)
				return (printf("malloc error get_str_quoted\n"), -1);
			else if (len == -2)
				return (printf("unmatch quotation\n"), -1);
			if (token_add_front(tokens, tmp) == NULL)
				return (printf("malloc error token_add_front\n"), -1);
			if (input[i] == '\'')
				(*tokens)->type = SINGLE_QUOTED_STR;
			else
				(*tokens)->type = DOUBLE_QUOTED_STR;
			i += len + 2;
		}
		else if (input[i] && !is_metachar(input[i]))
		{
			len = get_str_unquoted(input + i, &tmp);
			if (len == -1)
				return (printf("malloc error get_str_unquoted\n"), -1);
			if (token_add_front(tokens, tmp) == NULL)
				return (printf("malloc error token_add_front\n"), -1);
			(*tokens)->type = UNQUOTED_STR;
			i += len;
		}
		else if (input[i] && is_metachar(input[i]))
		{
			if (token_add_front(tokens, NULL) == NULL)
				return (printf("malloc error token_add_front\n"), -1);
			if (input[i] == '(')
			{
				(*tokens)->type = L_BRACKET;
				++i;
			}
			else if (input[i] == ')')
			{
				(*tokens)->type = R_BRACKET;
				++i;
			}
			else if (input[i] == '|' && input[i + 1] != '|')
			{
				(*tokens)->type = PIPE;
				++i;
			}
			else if (input[i] == '|' && input[i + 1] == '|')
			{
				(*tokens)->type = LOGIC_OR;
				i += 2;
			}
			else if (input[i] == '&' && input[i + 1] == '&')
			{
				(*tokens)->type = LOGIC_AND;
				i += 2;
			}
			else if (input[i] == '<' && input[i + 1] != '<')
			{
				(*tokens)->type = L_REDIR_TRUNC;
				++i;
			}
			else if (input[i] == '>' && input[i + 1] != '>')
			{
				(*tokens)->type = R_REDIR_TRUNC;
				++i;
			}
			else if (input[i] == '<' && input[i + 1] == '<')
			{
				(*tokens)->type = L_REDIR_APPEND;
				i += 2;
			}
			else if (input[i] == '>' && input[i + 1] == '>')
			{
				(*tokens)->type = R_REDIR_APPEND;
				i += 2;
			}
			else
			{
				// not sure what to do here but something NEEDS to be done // saw some people interpret this as string but maybe i should just send back an error message? like "unknown operand" or something
				(*tokens)->type = UNKNOWN; // send error message in this case
				++i;
			}
			// no alloc needed for string of token so set to NULL
			(*tokens)->data = NULL;
		}
		else
		{
			//printf("untreated character -> '%c'\n", input[i]);
			++i;
		}
		//else
		//{
		//	if (is_metachar(input[i]))
		//		get_meta_component(input + i, &tmp);
		//}
	}
	//*tokens = reverse_tokens(&tokenlist);
	//if (*tokens == NULL)
	//	return (destroy_tokenlist(&tokenlist), -1);
	reverse_tokens(tokens);
	return (0);
}

static void	display_token_type(t_tokenlist *token)
{
	printf("token type -> ");
	if (token->type == SINGLE_QUOTED_STR)
		printf("SINGLE_QUOTED_STR\n");
	else if (token->type == DOUBLE_QUOTED_STR)
		printf("DOUBLE_QUOTED_STR\n");
	else if (token->type == UNQUOTED_STR)
		printf("UNQUOTED_STR\n");
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

static void	display_tokens(t_tokenlist *begin)
{
	t_tokenlist	*curr;
	int			i;

	curr = begin;
	i = 0;
	while (curr)
	{
		printf("TOKEN #%d\n", i);
		printf("token data -> %s\n", curr->data);
		display_token_type(curr);
		printf("\n");
		curr = curr->next;
		++i;
	}
}

void	test_tokenizer(t_msh *msh)
{
	//int		i;
	int		ret;
	//char	**tokens;
	t_tokenlist	*tokens; // can be made into an ArrayList

	//if (msh->input[0] != '\'' && msh->input[0] != '"')
	//	return ((void)printf("Wrong input, fool!\n"));
	//(void)msh;
	//char	*test;
	//int	ret;
	////ret = get_str_unquoted(msh->input, &test);
	//ret = get_str_quoted(msh->input, &test);
	//if (ret == -1)
	//	return ((vo\nid)printf("Malloc error\n"));
	////else if (ret == -2)
	////	return ((void)printf("Unmatched quotation\n"));
	//printf("'%s'\nlen = %d\n", test, ret);
	//free(test);

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return (destroy_tokenlist(&tokens), (void)printf(MSH_ERROR ME_TOKEN_CMD));
	display_tokens(tokens);
	destroy_tokenlist(&tokens);
}
