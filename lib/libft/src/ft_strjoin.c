/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 21:08:54 by pabmart2          #+#    #+#             */
/*   Updated: 2025/03/31 22:06:41 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	char	*joined;

	if(!s1 && s2)
		return (ft_strdup(s2));
	else if(s1 && !s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined = malloc(sizeof(char) * len);
	if (!joined)
		return (NULL);
	joined[0] = '\0';
	ft_strlcat(joined, s1, len);
	ft_strlcat(joined, s2, len);
	return (joined);
}
