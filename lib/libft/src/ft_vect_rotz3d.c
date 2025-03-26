/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_rotz3d.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:51:16 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:37:30 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_rotz3d(double *vect, double *origin, double radians)
{
	double	*rotated_p;

	rotated_p = malloc(sizeof(double) * 3);
	if (!rotated_p)
		return (NULL);
	vect[0] -= origin[0];
	vect[1] -= origin[1];
	vect[2] -= origin[2];
	rotated_p[0] = (cos(radians) * vect[0]) - (sin(radians) * vect[1])
		+ origin[0];
	rotated_p[1] = (sin(radians) * vect[0]) + (cos(radians) * vect[1])
		+ origin[1];
	rotated_p[2] = vect[2] + origin[2];
	return (rotated_p);
}
