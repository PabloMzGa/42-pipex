/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_abs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:32:41 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:37:19 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_abs(double *vect, size_t d)
{
	double	*abs_vect;
	size_t	i;

	abs_vect = malloc(sizeof(double) * d);
	if (!abs_vect)
		return (NULL);
	i = 0;
	while (i < d)
	{
		abs_vect[i] = fabs(vect[i]);
		++i;
	}
	return (abs_vect);
}
