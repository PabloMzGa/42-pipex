/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:31:11 by pablo             #+#    #+#             */
/*   Updated: 2025/04/05 19:19:27 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Reads input from stdin until a specified EOF string is encountered.
 *
 * Continuously prompts the user with "heredoc >" and reads lines from stdin.
 * Lines are appended to a buffer until a line matching the EOF string is
 * encountered. The EOF comparison uses `ft_strncmp`. Returns the buffer
 * excluding the EOF line.
 *
 * @param eof The string marking the end of input (EOF).
 * @param eof_size The length of the EOF string.
 * @return A pointer to the buffer with input lines, or NULL on error.
 *
 * @note Memory:
 *
 * - Dynamically allocates memory for the buffer and strings.
 *
 * - Frees memory on errors or successful completion.
 *
 * Errors:
 *
 * - On read or join errors, frees memory, prints an error with `perror`,
 *   and returns NULL.
 */
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

int	heredoc_pipe(char *argv[], int heredoc_pipe[])
{
	char	*buffer;

	buffer = heredoc(argv[2], ft_strlen(argv[2]));
	if (!buffer)
		return (1);
	write(heredoc_pipe[1], buffer, ft_strlen(buffer));
	ft_free((void **)&buffer);
	if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
		return (perror("Error duplicating stdin in heredoc"), 1);
	return (0);
}
