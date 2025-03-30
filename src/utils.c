/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:08:35 by pablo             #+#    #+#             */
/*   Updated: 2025/03/30 01:43:56 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

void	check_files(char *argv[])
{
	if (ft_strncmp(argv[1], "here_doc", 9) != 0)
	{
		if (access(argv[1], F_OK) == -1)
			ft_perror("Error, infile does not exist", 0, EXIT_FAILURE);
		if (access(argv[1], R_OK) == -1)
			ft_perror("Error, infile can't be read", 0, EXIT_FAILURE);
	}
}

/**
 * @brief Closes both ends of a pipe and handles errors.
 *
 * This function attempts to close both ends of the pipe. If an error occurs
 * while closing either end, it prints an error message using perror and
 * sets the return status to -1.
 *
 * @param pipe An array of two file descriptors representing the pipe.
 *        pipe[0] is the read end, and pipe[1] is the write end.
 * @return 0 on success if both ends are closed without errors.
 *         -1 if an error occurs while closing either end of the pipe.
 */
int	close_pipe(int pipe[2])
{
	int	status;

	status = 0;
	if (close(pipe[0]) == -1)
	{
		status = -1;
		perror("Error closing pipe[0]");
	}
	if (close(pipe[1]) == -1)
	{
		status = -1;
		perror("Error closing pipe[1]");
	}
	return (status);
}

void	close_pipes(int pipes[][2])
{
	int	status;

	status = close_pipe(pipes[0]);
	status = close_pipe(pipes[1]);
	if (status == -1)
		exit(1);
}

char	*get_cmd_path(char command[], char **paths)
{
	char	*cmd;
	char	**splitted_args;
	char	*cmd_path;
	size_t	i;

	splitted_args = ft_split(command, ' ');
	if (!splitted_args)
		ft_perror("Error splitting arguments from command", ENOMEM,
			EXIT_FAILURE);
	i = 0;
	cmd = ft_strjoin("/", splitted_args[0]);
	if (!cmd)
	{
		ft_matrix_free((void **)splitted_args, 0);
		ft_perror("Error adding '/' to command", ENOMEM, EXIT_FAILURE);
	}
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			ft_matrix_free((void **)splitted_args, 0);
			ft_free((void **)&cmd);
			return (cmd_path);
		}
		if (cmd_path)
			ft_free((void **)&cmd_path);
		++i;
	}
	return (ft_free((void **)&cmd), ft_matrix_free((void **)splitted_args, 0),
		ft_matrix_free((void **)paths, 0), ft_perror("Command not found", 0,
			EXIT_FAILURE), NULL);
}

void	wait_childs(void)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
		pid = waitpid(-1, &status, 0);
	if (pid == -1 && errno != ECHILD)
		perror("Error al esperar a los procesos hijos");
}
