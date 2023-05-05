/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:33:09 by nlegrand          #+#    #+#             */
/*   Updated: 2022/09/15 01:29:25 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*beg;
	t_list	*tmp;
	t_list	*prev;

	beg = NULL;
	prev = NULL;
	if (lst && f && del)
	{
		while (lst != NULL)
		{
			tmp = (t_list *)malloc(sizeof(t_list));
			if (tmp == NULL)
				return (ft_lstclear(&beg, del), NULL);
			if (prev != NULL)
				prev->next = tmp;
			prev = tmp;
			if (beg == NULL)
				beg = tmp;
			tmp->content = (*f)(lst->content);
			lst = lst->next;
		}
		tmp->next = NULL;
	}
	return (beg);
}
