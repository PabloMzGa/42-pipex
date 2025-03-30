/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:34:12 by pablo             #+#    #+#             */
/*   Updated: 2025/03/30 01:43:23 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

void	fork_loop(int argc, char *argv[], int pipes[][2], char *paths[])
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
			return ;
		}
		else if (pid == -1)
			ft_perror("Error forking", 0, EXIT_FAILURE);
		else
			++i;
	}
	close_pipes(pipes);
	wait_childs();
	ft_matrix_free((void **)paths, 0);
}

int	main(int argc, char *argv[])
{
	char	**paths;
	int		pipes[3][2];

	if (argc < 5)
		ft_perror("Not enough arguments", EINVAL, 1);
	check_files(argv);
	paths = ft_split(ft_getenv("PATH"), ':');
	if (pipe(pipes[0]) == -1)
		ft_perror("Error creating the first pipe", 0, 1);
	if (pipe(pipes[1]) == -1)
		ft_perror("Error creating the second pipe", 0, 1);
	if (pipe(pipes[2]) == -1)
		ft_perror("Error creating heredoc pipe", 0, 1);
	fork_loop(argc, argv, pipes, paths);
}
