/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 12:56:44 by pablo             #+#    #+#             */
/*   Updated: 2025/03/31 23:00:53 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_perror(char message[], char err_val, char exit_value)
{
	if (err_val > 0)
		errno = err_val;
	perror(message);
	if (exit_value != 0)
	{
		ft_printf("Saliendo del programa desde ft_perror con estatus %i\n",
			exit_value);
		exit(exit_value);
	}
}
