/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_div.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:09:00 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:37:21 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_div(double *vect, double n, size_t d)
{
	size_t	i;
	double	*result;

	if (!vect)
		return (NULL);
	result = malloc(sizeof(double) * d);
	if (!result)
		return (NULL);
	i = 0;
	while (i < d)
	{
		result[i] = vect[i] / n;
		i++;
	}
	return (result);
}
