/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:37:31 by pablo             #+#    #+#             */
/*   Updated: 2025/05/23 11:55:38 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Cleans up resources and handles errors during command execution.
 *
 * This function is responsible for freeing the memory allocated for the
 * command path and its arguments, and then printing an error message
 * before exiting the program with a failure status.
 *
 * @param cmd_path Pointer to the command path string to be freed.
 * @param args Pointer to the array of argument strings to be freed.
 *
 */
static void	execution_cleanup(char *cmd_path, char **args)
{
	ft_free((void **)&cmd_path);
	ft_matrix_free((void **)args, 0);
	ft_perror("Error executing command", 0, 0);
}

/**
 * @brief Executes the first command in a pipeline
 *
 * This function handles execution of the first command in a pipeline setup.
 * It sets up input redirection from a file specified in argv[1], then
 * redirects standard output to the write end of the provided pipe.
 * After setup, the command is executed with execve.
 *
 * @param argv Array of command-line arguments:
 *             argv[1] -> Input file path |
 *             argv[2] -> Command with arguments to execute
 * @param paths String containing PATH environment data
 * @param pipe_fds Array with pipe file descriptors [read_end, write_end]
 *
 * @note The function does not return if command execution is successful
 *       as execve replaces the current process.
 * @note If any error occurs, appropriate cleanup is performed and
 *       the function exits with the corresponding error code.
 */
static void	execute_first_cmd(char *argv[], char **paths, int *pipe_fds)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;

	if (set_infile(argv[1]))
		return ;
	cmd_path = get_cmd_path(argv[2], paths);
	if (!cmd_path)
	{
		ft_matrix_free((void **)paths, 0);
		clean_pipe(pipe_fds);
		ft_perror("Command not found", 0, 127);
	}
	args = ft_split(argv[2], ' ');
	if (dup2(pipe_fds[1], STDOUT_FILENO) != -1)
	{
		ft_matrix_free((void **)paths, 0);
		clean_pipe(pipe_fds);
		execve(cmd_path, args, environ);
	}
	execution_cleanup(cmd_path, args);
}

/**
 * @brief Executes the last command in the pipe sequence
 *
 * This function handles the execution of the last command in a pipeline.
 * It redirects the standard input to read from the pipe and the standard
 * output to write to the specified output file. If any step fails, it
 * performs appropriate cleanup and error handling.
 *
 * @param argv Array of command line arguments containing commands and filenames
 * @param paths String containing the PATH environment variable content
 * @param pipe_fds Array of pipe file descriptors for interprocess communication
 *
 * @return None, but exits process on successful execution or handles errors
 */
static void	execute_last_cmd(char *argv[], char **paths, int *pipe_fds)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;

	if (set_outfile(argv[4]))
		return ;
	cmd_path = get_cmd_path(argv[3], paths);
	if (!cmd_path)
	{
		ft_matrix_free((void **)paths, 0);
		clean_pipe(pipe_fds);
		ft_perror("Command not found", 0, 127);
	}
	args = ft_split(argv[3], ' ');
	if (dup2(pipe_fds[0], STDIN_FILENO) != -1)
	{
		ft_matrix_free((void **)paths, 0);
		clean_pipe(pipe_fds);
		execve(cmd_path, args, environ);
	}
	execution_cleanup(cmd_path, args);
}

void	execute_cmd(char *argv[], char **paths, int *pipe_fds, int i)
{
	if (i == 2)
		execute_first_cmd(argv, paths, pipe_fds);
	else
		execute_last_cmd(argv, paths, pipe_fds);
	clean_pipe(pipe_fds);
	ft_matrix_free((void **)paths, 0);
}
