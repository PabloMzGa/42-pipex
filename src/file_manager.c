/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:16:02 by pablo             #+#    #+#             */
/*   Updated: 2025/03/30 01:12:36 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

void	set_infile(char file[])
{
	int	file_fd;

	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
		ft_perror("Error opening file", 0, EXIT_FAILURE);
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
