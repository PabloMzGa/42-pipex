/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:26:23 by pablo             #+#    #+#             */
/*   Updated: 2025/03/30 01:46:52 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

/**
 * @brief Processes the heredoc buffer by validating its size and reallocating
 *        memory as needed.
 *
 * This function performs the following operations:
 *
 * - If no bytes were read (`buffer_size_total[1] == 0`), it frees the buffer
 *   and terminates the program with an error message.
 *
 * - If the total bytes read are less than or equal to the size of the EOF
 *   marker (`eof_size`), it frees the buffer, allocates a new buffer of size 1,
 *   and sets it to an empty string.
 *
 * - Otherwise, it reallocates the buffer to exclude the EOF marker and appends
 *   a null terminator at the end.
 *
 * @param buffer_size_total Array where buffer_size_total[0] is the allocated
 *                          buffer size and buffer_size_total[1] is the total
 *                          data size in the buffer.
 * @param buffer A pointer to the heredoc buffer to be processed.
 * @param eof_size The size of the EOF marker to be excluded from the buffer.
 * @param buffer_size_total Array where buffer_size_total[0] is the allocated
 *                          buffer size and buffer_size_total[1] is the total
 *                          data size in the buffer.
 *
 * @note If memory allocation fails, the program terminates with
 *       an error message.
 */
static void	heredoc_processing(size_t buffer_size_total[2], char **buffer,
		size_t eof_size)
{
	if (buffer_size_total[1] == 0)
	{
		ft_free((void **)buffer);
		ft_perror("No bytes read from heredoc", ENODATA, EXIT_FAILURE);
	}
	if (buffer_size_total[1] <= eof_size)
	{
		ft_free((void **)buffer);
		*buffer = malloc(1);
		if (!(*buffer))
			ft_perror("Error reallocating heredoc buffer", ENOMEM,
				EXIT_FAILURE);
		(*buffer)[0] = '\0';
	}
	else
	{
		*buffer = ft_realloc(*buffer, buffer_size_total[0], buffer_size_total[0]
				- eof_size);
		if (!(*buffer))
			ft_perror("Error reallocating heredoc buffer", ENOMEM,
				EXIT_FAILURE);
		(*buffer)[buffer_size_total[1] - eof_size] = '\0';
	}
}

/**
 * @brief Processes the next line of input for a heredoc operation.
 *
 * Checks if the EOF marker is in the buffer. If found, returns 0 to signal
 * the end of heredoc input. If the buffer is nearing capacity, reallocates
 * it. Otherwise, prompts the user for more input.
 *
 * @param buffer_size_total Array where buffer_size_total[0] is the allocated
 *                          buffer size and buffer_size_total[1] is the total
 *                          data size in the buffer.
 * @param bytes_read Number of bytes read in the current operation.
 * @param buffer Pointer to the buffer containing the heredoc input.
 * @param eof EOF delimiter string to search for in the buffer.
 * @return int Returns 0 if EOF delimiter is found, otherwise 1.
 */
static int	heredoc_next_line(size_t buffer_size_total[2], ssize_t bytes_read,
		char **buffer, char *eof)
{
	buffer_size_total[1] += bytes_read;
	if (ft_strnstr(*buffer, eof, buffer_size_total[1]))
		return (0);
	else if (buffer_size_total[1] >= buffer_size_total[0] - 1)
		heredoc_buffer_realloc(&buffer_size_total[0], buffer);
	return (1);
}

/**

	* @brief Checks if the heredoc buffer contains the EOF delimiter and reallocates
 *        the buffer if necessary.
 *
 * Updates the total size of the buffer with the bytes read, then checks if the
 * EOF delimiter is present. If found, returns 0. Otherwise, reallocates the
 * buffer for more data and returns 1.
 *
 * @param buffer_size_total Array where buffer_size_total[0] is the allocated
 *                          buffer size and buffer_size_total[1] is the total
 *                          data size in the buffer.
 * @param bytes_read Number of bytes read in the current operation.
 * @param buffer Pointer to the buffer containing the heredoc input.
 * @param eof EOF delimiter string to search for in the buffer.
 * @return int Returns 0 if EOF delimiter is found, otherwise 1.
 */
static int	heredoc_buffer_full(size_t buffer_size_total[2], ssize_t bytes_read,
		char **buffer, char *eof)
{
	buffer_size_total[1] += bytes_read;
	if (ft_strnstr(*buffer, eof + 1, buffer_size_total[1]))
		return (0);
	heredoc_buffer_realloc(&buffer_size_total[0], buffer);
	return (1);
}

char	*heredoc(char *eof, size_t eof_size)
{
	char	*buffer;
	char	keep_reading;
	ssize_t	bytes_read;
	size_t	buffer_size_total[2];

	heredoc_init(buffer_size_total, &keep_reading, &buffer);
	while (keep_reading)
	{
		bytes_read = read(STDIN_FILENO, buffer + buffer_size_total[1],
				buffer_size_total[0] - buffer_size_total[1] - 1);
		heredoc_no_read(bytes_read, &keep_reading, &buffer);
		if (keep_reading && (size_t)bytes_read == buffer_size_total[0]
			- buffer_size_total[1] - 1)
			keep_reading = heredoc_buffer_full(buffer_size_total, bytes_read,
					&buffer, eof);
		else if (keep_reading)
			keep_reading = heredoc_next_line(buffer_size_total, bytes_read,
					&buffer, eof);
		if (keep_reading && buffer_size_total[1] > 0
			&& buffer[buffer_size_total[1] - 1] == '\n')
			ft_printf("heredoc> ");
	}
	heredoc_processing(buffer_size_total, &buffer, eof_size);
	return (buffer);
}
