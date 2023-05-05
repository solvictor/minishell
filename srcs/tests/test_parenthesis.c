/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_recur_parenthesis.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:20:21 by nlegrand          #+#    #+#             */
/*   Updated: 2023/03/19 08:17:32 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//static int	recur_parentheses(const char *line, int left)
//{
//	int	j;
//	int	errors;
//
//	errors = 0;
//	while (line[left] && line[left] != '(')
//		++left;
//	if (line[left] == '\0' && ++errors)
//		printf("[ERROR] didn't find \033[32mleft\033[0m parenthesis\n");
//	j = left + 1;
//	while (line[j] && line[j] != ')')
//	{
//		if (line[j] == '(')
//			j += recur_parentheses(line, j);
//		else
//			++j;
//	}
//	if (line[j] == '\0' && ++errors)
//		printf("[ERROR] didn't find \033[31mright\033[0m parenthesis\n");
//	if (!errors)
//	{
//		write(1, line + left, j - left + 1);
//		write(1, "\n", 1);
//	}
//	return (j - left + 1);
//}

static t_parse_stack	*push(t_parse_stack	**stack, int index)
{
	t_parse_stack	*tmp;

	if (stack == NULL)
		return (NULL);
	tmp = malloc(sizeof(t_parse_stack));
	if (tmp == NULL)
		return (NULL);
	tmp->index = index;
	tmp->next = *stack;
	*stack = tmp;
	return (tmp);
}

static void	pop(t_parse_stack **stack)
{
	t_parse_stack	*tmp;

	if (stack == NULL || *stack == NULL)
		return ;
	tmp = *stack;
	*stack = (*stack)->next;
	free(tmp);
}

static void	clear_stack(t_parse_stack **stack)
{
	t_parse_stack	*tmp;

	if (stack == NULL || *stack == NULL)
		return ;
	while (*stack)
	{
		tmp = *stack;
		*stack = (*stack)->next;
		free(tmp);
	}
}

static void	iter_parentheses(const char *line)
{
	t_parse_stack	*stack;
	int				i;

	stack = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == '(')
			push(&stack, i);
		if (line[i] == ')')
		{
			if (stack != NULL)
			{
				write(1, line + stack->index, i - stack->index + 1);
				write(1, "\n", 1);
				pop(&stack);
			}
			else
			{
				printf("\e[31mUnmatched right parenthesis\e[0m -> `%s'\n",
					line + i);
				clear_stack(&stack);
				return ;
			}
		}
		++i;
	}
	if (stack != NULL)
		printf("\e[34mUnmatched left parenthesis\e[0m -> `%s'\n",
			line + stack->index);
	if (stack)
		clear_stack(&stack);
}

void	test_parentheses(const char *line, int left)
{
	(void)line;
	(void)left;
	//recur_parentheses(line, left);
	iter_parentheses(line);
}
