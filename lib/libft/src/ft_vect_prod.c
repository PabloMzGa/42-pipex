/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_prod.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:52:10 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:38:17 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_prod(double *vect, double n, size_t d)
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
		result[i] = n * vect[i];
		i++;
	}
	return (result);
}
