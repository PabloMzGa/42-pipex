/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_ortproj.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:00:54 by pablo             #+#    #+#             */
/*   Updated: 2025/03/29 16:35:36 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_ortproj(double *a_point, double *normal, double *p_point,
		size_t dimension)
{
	double	*result;
	double	*vect_aux;
	double	n_aux;

	if (!dimension)
		return (NULL);
	vect_aux = ft_vect_sub(p_point, a_point, dimension);
	n_aux = ft_vect_dot(normal, vect_aux, dimension);
	ft_free((void **)&vect_aux);
	n_aux /= ft_vect_dot(normal, normal, dimension);
	vect_aux = ft_vect_prod(normal, n_aux, dimension);
	result = ft_vect_add(a_point, vect_aux, dimension);
	ft_free((void **)&vect_aux);
	return (result);
}
