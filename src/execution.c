/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:08:26 by pablo             #+#    #+#             */
/*   Updated: 2025/03/30 01:29:26 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

/**
 * @brief Handles the creation of a heredoc pipe and redirects its output to
 *        stdin.
 *
 * This function creates a heredoc by reading input until a specified delimiter
 * (EOF) is encountered. The input is then written to a pipe, and the pipe's
 * read end is redirected to the standard input (stdin). The function ensures
 * proper memory management and error handling throughout the process.
 *
 * - The function dynamically allocates memory for the EOF delimiter, appending
 *   newline characters to ensure proper matching.
 *
 * - It uses a helper function `heredoc` to read input until the delimiter is
 *   found.
 *
 * - A pipe is created to store the heredoc content, and the content is written
 *   to the pipe's write end.
 *
 * - The pipe's read end is then duplicated to stdin using `dup2`, allowing the
 *   heredoc content to be used as input for subsequent processes.
 *
 * - All allocated resources are freed, and the pipe is closed after use.
 *
 * @param argv[] The array of command-line arguments. The delimiter for the
 *               heredoc is expected to be in argv[2].
 *
 * @note If any error occurs during memory allocation, pipe creation, or other
 *       operations, the function will terminate the program with an error
 *       message.
 */
static void	heredoc_pipe(char *argv[], int heredoc_pipe[])
{
	char	*buffer;
	char	*eof;
	size_t	eof_s;

	eof_s = ft_strlen(argv[2]);
	eof = malloc(eof_s + 3);
	if (!eof)
		ft_perror("Error allocating EOF", 0, EXIT_FAILURE);
	ft_strlcpy(eof + 1, argv[2], eof_s);
	eof[0] = '\n';
	eof[eof_s + 1] = '\n';
	eof[eof_s + 2] = '\0';
	buffer = heredoc(eof, eof_s + 2);
	ft_printf("%s\n", buffer);
	ft_free((void **)&eof);
	if (pipe(heredoc_pipe) == -1)
		ft_errfree("Error creating heredoc pipe", 0, EXIT_FAILURE,
			(void *)buffer);
	write(heredoc_pipe[1], buffer, ft_strlen(buffer));
	ft_free((void **)&buffer);
	dup2(heredoc_pipe[0], STDIN_FILENO);
	close_pipe(heredoc_pipe);
}

/**
 * @brief Executes the first command in a pipeline.
 *
 * This function handles the execution of the first command in a pipeline. It
 * determines the command path, splits the command arguments, and sets up the
 * necessary file descriptors and pipes for execution. If the "here_doc" mode
 * is detected, it processes the heredoc input. Otherwise, it sets the input
 * file for the command. Finally, it executes the command using `execve`.
 *
 * @param argv Array of command-line arguments.
 * @param i Pointer to the current index of the command in the `argv` array.
 * @param pipes 2D array representing the pipe file descriptors.
 * @param paths Array of possible paths to search for the command executable.
 *
 * @note This function uses the global `environ` variable for environment
 *       variables.
 * @note If an error occurs during execution, the function will terminate
 *       the program with an error message and exit status.
 */
static void	execute_first_cmd(char *argv[], size_t *i, int pipes[][2],
		char **paths)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;

	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		heredoc_pipe(argv, pipes[2]);
		(*i)++;
	}
	else
		set_infile(argv[1]);
	cmd_path = get_cmd_path(argv[*i], paths);
	args = ft_split(argv[*i], ' ');
	dup2(pipes[0][1], STDOUT_FILENO);
	close_pipes(pipes);
	execve(cmd_path, args, environ);
	ft_free((void **)&cmd_path);
	ft_matrix_free((void **)args, 0);
	ft_matrix_free((void **)paths, 0);
	ft_perror("Error executing command", 0, EXIT_FAILURE);
}

/**
 * @brief Executes a middle command in a pipeline.
 *
 * This function sets up the necessary file descriptors for the middle command
 * in a pipeline, closes unused pipes, and executes the command using `execve`.
 * If the execution fails, it frees allocated resources and prints an error
 * message before exiting.
 *
 * @param argv Array of command-line arguments passed to the program.
 * @param i Index of the current command in the pipeline.
 * @param pipes 2D array representing the pipe file descriptors.
 *              Each row contains two file descriptors: one for reading and one
 *              for writing.
 * @param paths Array of possible paths to search for the command executable.
 *
 * @note This function assumes that `cmd_path` and `args` are properly
 *       initialized before calling `execve`. It also relies on the global
 *       `environ` variable
 *       for the environment.
 *
 * @warning This function does not return on success, as `execve` replaces the
 *          current process image. On failure, it terminates the process with
 *          an error message.
 */
static void	execute_middle_cmd(char *argv[], size_t i, int pipes[][2],
		char **paths)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;

	cmd_path = get_cmd_path(argv[i], paths);
	args = ft_split(argv[i], ' ');
	dup2(pipes[((i - 1) % 2)][0], STDIN_FILENO);
	dup2(pipes[i % 2][1], STDOUT_FILENO);
	close_pipes(pipes);
	execve(cmd_path, args, environ);
	ft_free((void **)&cmd_path);
	ft_matrix_free((void **)args, 0);
	ft_matrix_free((void **)paths, 0);
	ft_perror("Error executing command", 0, EXIT_FAILURE);
}

/**
 * @brief Executes the last command in a pipeline.
 *
 * This function sets up the output file, redirects the input from the
 * appropriate pipe, closes all pipes, and executes the specified command
 * using `execve`. If the execution fails, it frees allocated resources
 * and prints an error message before exiting.
 *
 * @param argv Array of command-line arguments.
 * @param i Index of the current command in the pipeline.
 * @param pipes 2D array representing the pipe file descriptors.
 * @param paths Array of possible paths for locating the command executable.
 *
 * @note This function assumes that `cmd_path` and `args` are properly
 *       initialized before calling `execve`. It also relies on the global
 *       `environ` variable
 *       for the environment.
 *
 * @warning This function does not return on success, as `execve` replaces the
 *          current process image. On failure, it terminates the process with
 *          an error message.
 */
static void	execute_last_cmd(char *argv[], size_t i, int pipes[][2],
		char **paths)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;

	cmd_path = get_cmd_path(argv[i], paths);
	args = ft_split(argv[i], ' ');
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		set_outfile(argv[i + 1], 1);
	else
		set_outfile(argv[i + 1], 0);
	dup2(pipes[((i - 1) % 2)][0], STDIN_FILENO);
	close_pipes(pipes);
	execve(cmd_path, args, environ);
	ft_free((void **)&cmd_path);
	ft_matrix_free((void **)args, 0);
	ft_matrix_free((void **)paths, 0);
	ft_perror("Error executing command", 0, EXIT_FAILURE);
}

void	execute_cmd(char *argv[], size_t *i, int pipes[][2], char **paths)
{
	if (*i == 2)
	{
		execute_first_cmd(argv, i, pipes, paths);
	}
	else if (argv[*i + 2] != NULL)
		execute_middle_cmd(argv, *i, pipes, paths);
	else
		execute_last_cmd(argv, *i, pipes, paths);
}
