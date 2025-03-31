/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:34:12 by pablo             #+#    #+#             */
/*   Updated: 2025/03/31 23:37:50 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

int	fork_loop(int argc, char *argv[], int pipes[][2], char *paths[])
{
	size_t	i;
	pid_t	pid;

	i = 2;
	while ((int)i < argc - 1)
	{
		pid = fork();
		if (pid == 0)
		{
			execute_cmd(argv, &i, pipes, paths);
			exit(1);
		}
		else if (pid == -1)
			ft_perror("Error forking", 0, EXIT_FAILURE);
		else
		{
			ft_printf("Fork con pid %i\n", pid);
			++i;
		}
	}
	close_pipes(pipes);
	ft_matrix_free((void **)paths, 0);
	return (wait_childs(pid));
}

int	main(int argc, char *argv[])
{
	char	**paths;
	int		pipes[3][2];
	int		exit_status;

	if (argc < 5)
		ft_perror("Not enough arguments", EINVAL, EXIT_FAILURE);
	// check_files(argv);
	paths = ft_split(ft_getenv("PATH"), ':');
	if (pipe(pipes[0]) == -1)
		ft_perror("Error creating the first pipe", 0, EXIT_FAILURE);
	if (pipe(pipes[1]) == -1)
		ft_perror("Error creating the second pipe", 0, EXIT_FAILURE);
	if (pipe(pipes[2]) == -1)
		ft_perror("Error creating heredoc pipe", 0, EXIT_FAILURE);
	exit_status = fork_loop(argc, argv, pipes, paths);
	ft_printf("Saliendo del programa con estatus %i\n", exit_status);
	return (exit_status);
}
