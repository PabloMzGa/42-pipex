/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:16:02 by pablo             #+#    #+#             */
/*   Updated: 2025/03/31 23:58:59 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

void	set_infile(char file[], int pipes[][2])
{
	int	file_fd;

	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
	{
		ft_perror("Error opening file", 0, EXIT_FAILURE);
		close_pipes(pipes);
	}
	dup2(file_fd, STDIN_FILENO);
	close(file_fd);
}

void	set_outfile(char file[], char append)
{
	int	file_fd;

	if (append)
		file_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
		ft_perror("Error opening file", 0, EXIT_FAILURE);
	dup2(file_fd, STDOUT_FILENO);
	close(file_fd);
}
