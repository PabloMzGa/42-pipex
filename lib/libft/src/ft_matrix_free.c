/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:17:34 by pablo             #+#    #+#             */
/*   Updated: 2025/05/22 11:53:58 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_matrix_free(void **array, size_t size)
{
	size_t	i;

	i = 0;
	if (size > 0)
	{
		while (i < size)
		{
			ft_free((void **)&array[i]);
			++i;
		}
	}
	else
	{
		while (array[i])
		{
			ft_free((void **)&array[i]);
			++i;
		}
	}
	ft_free((void **)&array);
}
