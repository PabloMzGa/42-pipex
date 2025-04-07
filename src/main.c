/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:59:19 by pablo             #+#    #+#             */
/*   Updated: 2025/04/07 13:19:39 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

int	main(int argc, char *argv[])
{
	size_t	n_pipes;
	int		**pipes;

	if (argc < 5)
		ft_perror("Not enough arguments", EINVAL, EXIT_FAILURE);
	n_pipes = argc - 4;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		++n_pipes;
	pipes = ft_calloc(n_pipes + 1, sizeof(int *));
	if (create_pipes(pipes, n_pipes))
		ft_perror("Error creating pipes", 0, EXIT_FAILURE);
	return (fork_loop(argc, argv, pipes));
}
