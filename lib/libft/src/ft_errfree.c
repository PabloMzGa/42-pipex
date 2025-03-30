/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:38:43 by pablo             #+#    #+#             */
/*   Updated: 2025/03/29 15:05:42 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_errfree(char *msg, char err, char exit, void **ptr)
{
	ft_free((void *)ptr);
	ft_perror(msg, err, exit);
}
