/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_magn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:55:22 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:37:26 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_vect_magn(double *vect, size_t d)
{
	size_t	i;
	double	res;

	res = 0;
	i = 0;
	while (i < d)
	{
		res += pow(vect[i], 2);
		++i;
	}
	res = sqrt(res);
	return (res);
}
