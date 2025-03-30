/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:24:18 by pablo             #+#    #+#             */
/*   Updated: 2025/03/29 13:58:53 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

void	heredoc_buffer_realloc(size_t *buffer_size, char **buffer)
{
	*buffer = ft_realloc(*buffer, *buffer_size, *buffer_size * 2);
	if (!(*buffer))
		ft_perror("Error reallocating heredoc buffer", ENOMEM, EXIT_FAILURE);
	*buffer_size *= 2;
}

void	heredoc_init(size_t buffer_size_total[2], char *keep_reading,
		char **buffer)
{
	buffer_size_total[0] = HEREDOC_BUFFER_SIZE;
	*buffer = malloc(buffer_size_total[0]);
	if (!(*buffer))
		ft_perror("Error allocating heredoc buffer", ENOMEM, EXIT_FAILURE);
	*keep_reading = 1;
	buffer_size_total[1] = 0;
	ft_printf("heredoc> ");
}

void	heredoc_no_read(ssize_t bytes_read, char *keep_reading, char **buffer)
{
	if (bytes_read == -1)
		ft_errfree("Error reading heredoc", 0, EXIT_FAILURE, (void **)buffer);
	else if (bytes_read == 0)
		*keep_reading = 0;
}
