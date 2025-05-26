/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:10:05 by pablo             #+#    #+#             */
/*   Updated: 2025/05/22 22:17:53 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	set_infile(char file[])
{
	int	file_fd;

	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
	{
		perror("Error opening infile");
		return (1);
	}
	if (dup2(file_fd, STDIN_FILENO) == -1)
	{
		perror("Error duplicating file");
		return (1);
	}
	if (close(file_fd) == -1)
	{
		perror("Error closing file");
		return (1);
	}
	return (0);
}

void	remove_heredoc_tmp_file(char *filename)
{
	if (unlink(filename) != 0)
		perror("Error removing heredoc temp file");
}

int	set_outfile(char file[], char append)
{
	int	file_fd;

	if (append)
		file_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		perror("Error opening outfile");
		return (1);
	}
	if (dup2(file_fd, STDOUT_FILENO) == -1)
	{
		perror("Error duplicating file");
		if (close(file_fd))
			perror("Error closing file");
		return (1);
	}
	if (close(file_fd))
		return (perror("Error closing file"), 1);
	return (0);
}
