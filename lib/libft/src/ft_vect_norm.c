/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vect_norm.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:51:16 by pablo             #+#    #+#             */
/*   Updated: 2025/03/14 17:37:28 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_vect_norm(double *vect, size_t d)
{
	return (ft_vect_div(vect, ft_vect_magn(vect, d), d));
}
