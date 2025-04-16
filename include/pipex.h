/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:33:49 by pablo             #+#    #+#             */
/*   Updated: 2025/04/16 19:25:25 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft.h"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/**
 * @brief Cleans up and closes an array of pipes.
 *
 * This function iterates through an array of pipes,
	closing both ends of each pipe
 * and freeing the associated memory. If any `close` operation fails, it sets a
 * status flag and reports a fatal error using `ft_perror`.
 *
 * @param pipes A pointer to an array of integer pointers, where each element
 *              represents a pipe (an array of two file descriptors).
 *
 * @note The function assumes that the `pipes` array is NULL-terminated.
 *       It also frees the memory allocated for the `pipes` array itself.
 *       If any error occurs while closing the pipes, the program will terminate
 *       with an error message.
 */
void	clean_pipes(int **pipes);

/**
 * @brief Creates a specified number of pipes and allocates memory for them.
 *
 * Allocates memory for an array of pipes and initializes each pipe using the
 * `pipe()` system call. If memory allocation or pipe creation fails, it cleans
 * up any previously allocated resources and returns an error.
 *
 * @param pipes A pointer to an array of integer pointers where each element
 *              represents a pipe (an array of two integers for read/write ends).
 * @param n_pipes The number of pipes to create.
 *
 * @return 0 on success, 1 on failure. On failure, an error message is printed
 *         using `perror()` and previously allocated resources are freed.
 *
 * @note The caller must free the allocated memory for the pipes using the
 *       `clean_pipes()` function after use.
 */
int		create_pipes(int **pipes, size_t n_pipes);

/**
 * @brief Executes a command based on its position in a pipeline.
 *
 * Determines whether the command is the first, middle, or last in a sequence
 * of piped commands and calls the appropriate execution function. Handles
 * cleanup of allocated resources such as paths and pipes.
 *
 * @param argv Array of command-line arguments.
 * @param i Pointer to the index of the current command in the pipeline. If
 *          *i is -1, it is updated to 3 to indicate the first command.
 * @param pipes Double pointer to an array of pipes used for inter-process
 *              communication.
 * @param paths Array of possible paths for locating the command executables.
 *
 * @note Assumes that pipes and paths are already set up. Frees paths and
 *       cleans up pipes after execution.
 */

void	execute_cmd(char *argv[], int *i, int **pipes, char **paths);

/**
 * @brief Resolves the full path of a command by searching in the given paths.
 *
 * This function takes a command string, which may include arguments, and an
 * array of directory paths to search for the command. It splits the command
 * into its arguments, extracts the command name, and appends a '/' to it. The
 * function then iterates through the provided paths to locate the command. If
 * the command is found, its full path is returned. If an error occurs (e.g.,
 * memory allocation failure or empty command), appropriate error handling is
 * performed.
 *
 * @param command A string containing the command to resolve. It may include
 *                arguments.
 * @param paths An array of strings representing the directories to search for
 *              the command.
 *
 * @return A string containing the full path of the command if found, or NULL
 *         if an error occurs. The returned string must be freed by the caller.
 */
char	*get_cmd_path(char command[], char **paths);

/**
 * @brief Executes a loop to fork processes and handle commands.
 *
 * This function performs the following steps:
 *
 * - Splits the PATH environment variable into an array of paths.
 *
 * - Checks if the first argument is "here_doc" to adjust the starting index.
 *
 * - Iterates through commands, forking a process for each and handling it.
 *
 * - Cleans up resources such as pipes and the paths array.
 *
 * - Waits for all child processes to finish and returns their status.
 *
 * @param argc The argument count passed to the program.
 * @param argv The argument vector containing command-line arguments.
 * @param pipes A double pointer to an array of pipes used for inter-process
 *              communication.
 *
 * @return The status of the child processes after they have all completed.
 *
 * @note If an error occurs while retrieving the PATH or during resource
 *       allocation, the function cleans up and exits with a failure status.
 */
int		fork_loop(int argc, char *argv[], int **pipes);

/**
 * @brief Reads input from stdin until a specified EOF string is encountered.
 *
 * Continuously prompts the user with "heredoc >" and reads lines from stdin.
 * Lines are appended to a buffer until a line matching the EOF string is
 * encountered. The EOF comparison uses `ft_strncmp`. Returns the buffer
 * excluding the EOF line.
 *
 * @param eof The string marking the end of input (EOF).
 * @param eof_size The length of the EOF string.
 * @return A pointer to the buffer with input lines, or NULL on error.
 *
 * @note Memory:
 *
 * - Dynamically allocates memory for the buffer and strings.
 *
 * - Frees memory on errors or successful completion.
 *
 * Errors:
 *
 * - On read or join errors, frees memory, prints an error with `perror`,
 *   and returns NULL.
 */
char	*heredoc(char *eof, size_t eof_size);

/**
 * @brief Handles heredoc functionality by reading input until a delimiter
 *        is encountered and writing it to a pipe.
 * Reads input from the user until the specified delimiter (argv[2]) is
 * encountered. The input is stored in a buffer, written to the write end of the
 * pipe (heredoc_pipe[1]), and freed. The read end of the pipe (heredoc_pipe[0])
 * is duplicated to standard input (STDIN_FILENO) for subsequent processes to
 * read heredoc content.
 *
 * @param argv Array of command-line arguments. The delimiter for the heredoc
 *             is expected to be in argv[2].
 * @param heredoc_pipe An array of two file descriptors representing the pipe.
 *                     heredoc_pipe[1] is used for writing the heredoc content,
 *                     and heredoc_pipe[0] is used for reading.
 *
 * @return 0 on success or 1 if an error occurs
 *         (e.g., memory allocation failure in heredoc()).
 */
int		heredoc_pipe(char *argv[], int heredoc_pipe[]);

/**
 * @brief Sets the specified file as the standard input (stdin) for the process.
 *
 * This function opens the given file in write-only mode, creates it if it
 * doesn't exist, truncates it if it does, duplicates its file descriptor to
 * replace the standard output (stdout), and then closes the original file
 * descriptor. If any of these operations fail, an error message is printed
 * to stderr using perror, and the function returns 1 to indicate failure.
 *
 * @param file The path to the file to be used as the standard output.
 * @return int Returns 0 on success, or 1 if an error occurs.
 *
 * @note This function modifies the standard output (stdout) of the process, so
 *       subsequent writes to stdout will write to the specified file.
 */
int		set_infile(char file[]);

/**
 * @brief Sets the specified file as the standard output (STDOUT).
 *
 * Opens the given file with the specified mode (append or truncate),
 * duplicates its file descriptor to STDOUT_FILENO, and closes the
 * original file descriptor. If any operation fails, an error message
 * is printed to stderr using perror, and the function returns an error
 * code.
 *
 * @param file The path to the file to be used as the output.
 * @param append A flag indicating whether to append to the file
 *               (non-zero value) or truncate the file (zero value).
 * @return int Returns 0 on success, or 1 if an error occurs.
 *
 * @note The file is created if it does not exist, with permissions
 *       set to 0644. If the file cannot be opened, duplicated, or
 *       closed, an error message is printed, and the function
 *       returns an error code.
 */
int		set_outfile(char file[], char append);

/**
 * @brief Waits for all child processes to terminate and retrieves the exit
 *        status of the last child process specified by `last_pid`.
 *
 * This function waits for all child processes to terminate using `waitpid`
 * in a loop. If the PID of the terminated child matches `last_pid`, the
 * function retrieves its exit status. The exit status is determined based
 * on whether the child exited normally or was terminated by a signal.
 *
 * If an error occurs while waiting for child processes (other than no more
 * child processes to wait for), an error message is printed using `perror`.
 *
 * @param last_pid The PID of the last child process whose exit status is to
 *                 be returned.
 * @return The exit status of the child process with PID `last_pid`. If no
 *         such process is found, the function returns 0.
 */
int		wait_childs(pid_t last_pid);

#endif
