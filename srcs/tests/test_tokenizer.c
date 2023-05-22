/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vegret <victor.egret.pro@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 16:25:37 by nlegrand          #+#    #+#             */
/*   Updated: 2023/05/22 19:18:14 by vegret           ###   ########.fr       */
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

//static void	copy_str(const char *str, char *dst)
//{
//	int	i;
//}

static int	strlen_quoted(const char *str)
{

	const char	limiter = *str;
	int			len;

	++str;
	len = 0;
	while (str[len] && str[len] != limiter)
		++len;
	if (str[len] != limiter)
		return (-1);
	return (len);
}
static int	strlen_unquoted(const char *str)
{
	int	len;

	len = 0;
	while (str[len] && (!is_whitespace(str[len]) && !is_metachar(str[len])
		&& str[len] != '\'' && str[len] != '"'))
		++len;
	return (len);
}

static int	get_strlen(const char *str)
{
	int	i;
	int	len;
	int	ret;

	i = 0;
	len = 0;
	while (str[i] && !is_metachar(str[i]) && !is_whitespace(str[i]))
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			ret = strlen_quoted(str + i);
			if (ret == -1)
				return (-1);
			len += ret;
			i += ret + 2;
		}
		else
		{
			ret = strlen_unquoted(str + i);
			len += ret;
			i += ret;
		}
	}
	return (len);
}

static int	copy_str(const char *str, char *dst)
{
	int	i;
	int	len;
	int	ret;

	i = 0;
	len = 0;
	while (str[i] && !is_metachar(str[i]) && !is_whitespace(str[i]))
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			ret = strlen_quoted(str + i);
			ft_memcpy(dst + len, str + i + 1, ret);
			len += ret;
			i += ret + 2;
		}
		else
		{
			ret = strlen_unquoted(str + i);
			ft_memcpy(dst + len, str + i, ret);
			len += ret;
			i += ret;
		}
	}
	dst[len] = '\0';
	return (i);
}

static int	get_str(const char *str, char **dst)
{
	int	len;

	len = get_strlen(str);
	if (len == -1)
		return (-2);
	*dst = malloc(sizeof(char) * (len + 1));
	if (*dst == NULL)
		return (-1);
	return (copy_str(str, *dst));
}

int	tokenize(t_msh *msh, const char *input, t_tokenlist **tokens)
{
	//t_tokenlist	*tokenlist; // can be replaced with t_list from libft and turned into an array of tokenlist later
	char		*tmp;
	int			i;
	int			ret;

	(void) msh;
	*tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			++i;
		//if (input[i] && !is_metachar(input[i]) &&
		//	(input[i] == '\'' || input[i] == '"'))
		if (input[i] && !is_metachar(input[i]))
		{
			ret = get_str(input + i, &tmp);
			if (ret == -1)
				return (printf("malloc error get_str_quoted\n"), -1);
			else if (ret == -2)
				return (printf("unmatch quotation\n"), -1);
			if (token_add_front(tokens, tmp) == NULL)
				return (free(tmp), printf("malloc error token_add_front\n"),
					-1);
			(*tokens)->type = STR;
			i += ret;
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
	if (token->type == STR)
		printf("STR\n");
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
		printf("token data -> %s\n", curr->data);
		display_token_type(curr);
		printf("\n");
		curr = curr->next;
		++i;
	}
}

typedef struct s_tree t_tree;

struct s_tree {
	t_tokenlist	*cmd;
	t_tree		*left;
	t_tree		*right;
};

t_tree	*new_tree(void)
{
	t_tree	*new;

	new = malloc(sizeof(t_tree));
	new->cmd = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}


t_tree *create_tree(t_tokenlist *tokens)
{
	t_tree *tree = NULL;
	t_tokenlist *current = tokens;

	while (current)
	{
		if (current->type == L_BRACKET)
		{
			
		}
		current = current->next;
	}
	return tree;
}

void	test_tokenizer(t_msh *msh)
{
	int		ret;
	t_tokenlist	*tokens; // can be made into an ArrayList

	ret = tokenize(msh, msh->input, &tokens);
	if (ret < 0)
		return (destroy_tokenlist(&tokens), (void)printf(MSH_ERROR ME_TOKEN_CMD));
	display_tokens(tokens);
	t_tree *t = create_tree(tokens);
	destroy_tokenlist(&tokens);
}
