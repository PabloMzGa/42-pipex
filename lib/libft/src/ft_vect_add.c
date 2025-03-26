/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_add.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:52:10 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:37:41 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_add(double *v1, double *v2, size_t d)
{
	size_t	i;
	double	*result;

	if (!v1 || !v2)
		return (NULL);
	result = malloc(sizeof(double) * d);
	if (!result)
		return (NULL);
	i = 0;
	while (i < d)
	{
		result[i] = v1[i] + v2[i];
		++i;
	}
	return (result);
}
