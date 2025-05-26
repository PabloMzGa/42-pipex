/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:59:19 by pablo             #+#    #+#             */
/*   Updated: 2025/05/26 19:32:42 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

int	main(int argc, char *argv[])
{
	int	*pipe_fds;

	if (argc < 5)
		ft_perror("Not enough arguments", EINVAL, EXIT_FAILURE);
	else if (argc > 5)
		ft_perror("Too many arguments", EINVAL, EXIT_FAILURE);
	pipe_fds = create_pipe();
	if (!pipe_fds)
		ft_perror("Error creating pipes", 0, EXIT_FAILURE);
	return (fork_loop(argc, argv, pipe_fds));
}
