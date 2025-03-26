/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_dot.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 20:04:26 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:38:13 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_vect_dot(double *v1, double *v2, size_t d)
{
	size_t	i;
	double	result;

	i = 0;
	result = 0;
	while (i < d)
	{
		result += v1[i] * v2[i];
		++i;
	}
	return (result);
}
