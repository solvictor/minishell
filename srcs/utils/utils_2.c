/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlegrand <nlegrand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:28:05 by nlegrand          #+#    #+#             */
/*   Updated: 2023/07/03 17:34:59 by nlegrand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Closes all file descriptors not equal to -1 int the array
void	close_valid_fds(int	*arr, int size)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (i < size)
	{
		if (arr[i] != -1)
		{
			close(arr[i]);
			arr[i] = -1;
		}
		++i;
	}
}
