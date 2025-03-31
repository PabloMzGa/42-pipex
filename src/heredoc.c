/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:26:23 by pablo             #+#    #+#             */
/*   Updated: 2025/03/31 22:19:02 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

char	*heredoc(char *eof, size_t eof_size)
{
	char	*buffer;
	char	*line;
	char	*tmp;

	buffer = NULL;
	while (1)
	{
		ft_printf("heredoc >");
		line = ft_get_next_line(STDIN_FILENO);
		if (line)
		{
			if (!ft_strncmp(line, eof, eof_size))
				return (ft_free((void **)&line), buffer);
			tmp = buffer;
			buffer = ft_strjoin(buffer, line);
			if (tmp)
				ft_free((void **)&tmp);
			ft_free((void **)&line);
			if (!buffer)
				return (ft_free((void **)&buffer),
					perror("Error joining heredoc"), NULL);
		}
		else
			return (ft_free((void **)&buffer), perror("Heredoc error"), NULL);
	}
}
