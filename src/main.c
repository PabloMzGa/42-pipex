/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:59:19 by pablo             #+#    #+#             */
/*   Updated: 2025/04/16 19:30:58 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

int	main(int argc, char *argv[])
{
	size_t	n_pipes;
	int		**pipes;

	n_pipes = argc - 4;
	if (argc > 2 && ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		++n_pipes;
		if (argc < 6)
			ft_perror("Not enough arguments", EINVAL, EXIT_FAILURE);
	}
	else if (argc < 5)
		ft_perror("Not enough arguments", EINVAL, EXIT_FAILURE);
	pipes = ft_calloc(n_pipes + 1, sizeof(int *));
	if (create_pipes(pipes, n_pipes))
		ft_perror("Error creating pipes", 0, EXIT_FAILURE);
	return (fork_loop(argc, argv, pipes));
}
