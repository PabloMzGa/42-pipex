/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:17:34 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:36:59 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_matrix_free(void **array, size_t size)
{
	size_t	i;

	i = 0;
	if (i > 0)
	{
		while (i < size)
		{
			free(array[i]);
			++i;
		}
	}
	else
	{
		while (array[i])
		{
			free(array[i]);
			++i;
		}
	}
	free(array);
}
