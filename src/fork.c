/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:16:10 by pablo             #+#    #+#             */
/*   Updated: 2025/05/22 22:13:13 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Creates a child process to execute a command within a pipeline
 *
 * This function forks a new process. In the child process, it executes
 * the specified command using execute_cmd. In the parent process, it returns
 * the child's PID. If fork fails, it cleans up resources and exits with an
 * error.
 *
 * @param i Index of the command in the pipeline
 * @param argv Array of arguments, including the command to execute
 * @param pipe_fds Array of pipe file descriptors for the pipeline
 * @param paths Matrix containing PATH information for executable lookup
 *
 * @return The process ID of the child, or -1 if fork failed
 */
static pid_t	handle_fork(int i, char *argv[], int *pipe_fds, char **paths)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execute_cmd(argv, paths, pipe_fds, i);
		exit(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		ft_perror("Error forking", 0, EXIT_FAILURE);
		clean_pipe(pipe_fds);
		ft_matrix_free((void **)paths, 0);
	}
	return (pid);
}

int	fork_loop(int argc, char *argv[], int *pipe_fds)
{
	int		i;
	pid_t	pid;
	char	**paths;

	paths = ft_split(ft_getenv("PATH"), ':');
	if (!paths)
	{
		clean_pipe(pipe_fds);
		ft_perror("Error getting cmd paths", 0, EXIT_FAILURE);
	}
	i = 2;
	while (i < argc - 1)
	{
		pid = handle_fork(i, argv, pipe_fds, paths);
		++i;
	}
	ft_matrix_free((void **)paths, 0);
	clean_pipe(pipe_fds);
	return (wait_childs(pid));
}
