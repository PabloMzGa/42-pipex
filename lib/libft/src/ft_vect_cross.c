/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_cross.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:04:26 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:37:24 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_cross(double *v1, double *v2, size_t d)
{
	double	*result;

	if (d != 3)
		return (perror("Cross product only work in 3D"), NULL);
	result = malloc(sizeof(double) * d);
	if (!result)
		return (NULL);
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return (result);
}
