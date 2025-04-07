/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:16:10 by pablo             #+#    #+#             */
/*   Updated: 2025/04/07 13:18:45 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Handles the creation of a new process using fork and executes a
 *        command in the child process.
 *
 * This function forks the current process. In the child process, it calls
 * `execute_cmd` to execute a command and then exits. In the parent process,
 * it checks for fork errors and handles cleanup if an error occurs.
 *
 * @param i Index of the command to execute from the argv array.
 * @param argv Array of command-line arguments passed to the program.
 * @param pipes A 2D array of pipe file descriptors used for inter-process
 *              communication.
 * @param paths Array of possible paths to search for the executable command.
 *
 * @note If fork fails, the function cleans up allocated resources and exits
 *       the program with an error message.
 */
static void	handle_fork(int i, char *argv[], int **pipes, char **paths)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execute_cmd(argv, i, pipes, paths);
		exit(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		ft_matrix_free((void **)paths, 0);
		clean_pipes(pipes);
		ft_perror("Error forking", 0, EXIT_FAILURE);
	}
}

int	fork_loop(int argc, char *argv[], int **pipes)
{
	int		i;
	char	**paths;

	i = 2;
	paths = ft_split(ft_getenv("PATH"), ':');
	if (!paths)
	{
		clean_pipes(pipes);
		ft_perror("Error getting cmd paths", 0, EXIT_FAILURE);
	}
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		i = -1;
	while (i < argc - 1)
	{
		handle_fork(i, argv, pipes, paths);
		if (i == -1)
			i = 3;
		++i;
	}
	clean_pipes(pipes);
	ft_matrix_free((void **)paths, 0);
	return (wait_childs(0));
}
