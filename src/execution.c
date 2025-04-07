/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:37:31 by pablo             #+#    #+#             */
/*   Updated: 2025/04/07 13:19:42 by pablo            ###   ########.fr       */
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
 * @brief Executes the first command in a pipeline.
 *
 * This function handles the execution of the first command in a pipeline. It
 * performs the following steps:
 *
 * - If the command is the result of a heredoc, it sets up the appropriate pipe.
 *
 * - Otherwise, it sets up the input file for the command.
 *
 * - Resolves the command's executable path using the provided paths.
 *
 * - Splits the command into arguments.
 *
 * - Redirects the standard output to the appropriate pipe.
 *
 * - Executes the command using `execve`.
 *
 * @param argv An array of strings representing the command-line arguments.
 * @param i The index of the current command in the pipeline.
 * @param pipes A double pointer to an array of pipes for inter-process
 *              communication.
 * @param paths An array of strings representing the possible paths to search
 *              for the command.
 *
 * @note
 * - The function uses the global `environ` variable to pass the environment
 *   variables to the executed command.
 *
 * - The function assumes that `pipes` and `paths` are properly initialized.
 *
 * - If the command is not found, it exits with an error code of 127.
 *
 * @warning This function does not return if `execve` is successful, as the
 *         current process image is replaced by the new process image.
 */
static void	execute_first_cmd(char *argv[], int i, int **pipes, char **paths)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;

	if (i == 3)
	{
		if (heredoc_pipe(argv, pipes[0]))
			return ;
	}
	else if (set_infile(argv[1]))
		return ;
	cmd_path = get_cmd_path(argv[i], paths);
	if (!cmd_path)
	{
		clean_pipes(pipes);
		ft_perror("Command not found", 0, 127);
	}
	args = ft_split(argv[i], ' ');
	if (dup2(pipes[i - 2][1], STDOUT_FILENO) != -1)
	{
		clean_pipes(pipes);
		execve(cmd_path, args, environ);
	}
	execution_cleanup(cmd_path, args);
}

/**
 * @brief Executes a middle command in a pipeline.
 *
 * This function handles the execution of a command that is neither the first
 * nor the last in a pipeline. It resolves the command's executable path, sets
 * up input and output redirection using pipes, and executes the command using
 * `execve`.
 *
 * - If the executable path cannot be resolved, the function cleans up the
 *   pipes and terminates the process with an error message and exit code 127.
 *
 * - The command string is split into arguments using `ft_split`.
 *
 * - Input redirection is set up using `pipes[i - 3][0]`, and output redirection
 *   is set up using `pipes[i - 2][1]` with `dup2`.
 *
 * - If `execve` fails, the function ensures proper cleanup of allocated
 *   resources before terminating.
 *
 * @param argv Array of command-line arguments passed to the program.
 *             The command to be executed is located at index `i`.
 * @param i Index of the current command in the `argv` array.
 * @param pipes A 2D array of pipe file descriptors used for inter-process
 *              communication. The function uses `pipes[i - 3][0]` for input
 *              and `pipes[i - 2][1]` for output.
 * @param paths An array of possible paths where the command's executable
 *              might be located. Used to resolve the command's full path.
 *
 * @note
 * - The function uses the global `environ` variable to pass the environment
 *   variables to the executed command.
 *
 * - The function assumes that `pipes` and `paths` are properly initialized.
 *
 * - If the command is not found, it exits with an error code of 127.
 *
 * @warning This function does not return if `execve` is successful, as the
 *          current process image is replaced by the new process image.
 */
static void	execute_middle_cmd(char *argv[], int i, int **pipes, char **paths)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;

	cmd_path = get_cmd_path(argv[i], paths);
	if (!cmd_path)
	{
		clean_pipes(pipes);
		ft_perror("Command not found", 0, 127);
	}
	args = ft_split(argv[i], ' ');
	if (dup2(pipes[i - 3][0], STDIN_FILENO) != -1 && dup2(pipes[i - 2][1],
			STDOUT_FILENO) != -1)
	{
		clean_pipes(pipes);
		execve(cmd_path, args, environ);
	}
	execution_cleanup(cmd_path, args);
}

/**
 * @brief Executes the last command in a pipeline.
 *
 * This function is responsible for executing the final command in a pipeline,
 * managing output redirection, and invoking the command using `execve`. It
 * ensures proper cleanup of resources such as pipes and dynamically allocated
 * memory after execution.
 *
 * - If the first argument (`argv[1]`) is "here_doc", the output file is opened
 *   in append mode; otherwise, it is opened in overwrite mode.
 *
 * - The function exits early if the output file cannot be opened or set up.
 *
 * - It determines the command path using `get_cmd_path` and splits the command
 *   arguments using `ft_split`.
 *
 * - The standard input is redirected to the read end of the pipe corresponding
 *   to the current command.
 *
 * - If the command path is invalid or `execve` fails, the function performs
 *   cleanup and terminates with an error.
 *
 * @param argv Array of command-line arguments.
 * @param i Index of the current command in the `argv` array.
 * @param pipes A 2D array of pipe file descriptors used for inter-process
 *              communication.
 * @param paths Array of possible paths to search for the command executable.
 *
 * @note
 * - The function uses the global `environ` variable to pass the environment
 *   variables to the executed command.
 *
 * - The function assumes that `pipes` and `paths` are properly initialized.
 *
 * - If the command is not found, it exits with an error code of 127.
 *
 * @warning The function terminates the process if an error occurs during
 *          execution or resource setup.
 */
static void	execute_last_cmd(char *argv[], int i, int **pipes, char **paths)
{
	extern char	**environ;
	char		**args;
	char		*cmd_path;
	int			outfile_status;

	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		outfile_status = set_outfile(argv[i + 1], 1);
	else
		outfile_status = set_outfile(argv[i + 1], 0);
	if (!outfile_status)
	{
		cmd_path = get_cmd_path(argv[i], paths);
		if (!cmd_path)
		{
			clean_pipes(pipes);
			ft_perror("Command not found", 0, 127);
		}
		args = ft_split(argv[i], ' ');
		if (dup2(pipes[i - 3][0], STDIN_FILENO) != -1)
		{
			clean_pipes(pipes);
			execve(cmd_path, args, environ);
		}
		execution_cleanup(cmd_path, args);
	}
}

void	execute_cmd(char *argv[], int i, int **pipes, char **paths)
{
	if (i == -1 || i == 2)
	{
		if (i == -1)
			i = 3;
		execute_first_cmd(argv, i, pipes, paths);
	}
	else if (argv[i + 2] != NULL)
		execute_middle_cmd(argv, i, pipes, paths);
	else
		execute_last_cmd(argv, i, pipes, paths);
	ft_matrix_free((void **)paths, 0);
	clean_pipes(pipes);
}
