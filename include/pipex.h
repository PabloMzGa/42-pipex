/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:33:49 by pablo             #+#    #+#             */
/*   Updated: 2025/03/30 01:44:03 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define HEREDOC_BUFFER_SIZE 5

/**
 * @brief Checks the accessibility of the input and output files.
 *
 * This function verifies the existence and permissions of the input file
 * (argv[1]) and the output file (argv[argc - 1]). If any of the checks fail,
 * it calls the `ft_perror` function to display an error message and terminate
 * the program.
 *
 *		Error cases:
 *
 * - If the input file does not exist, an error message is displayed.
 *
 * - If the input file cannot be read, an error message is displayed.
 *
 * @param argv The argument vector containing file paths.
 * @note It closes the program if an error occurs
 */
void	check_files(char *argv[]);

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
int		close_pipe(int pipe[2]);

/**
 * @brief Closes two pipes represented as an array of file descriptors.
 *
 * This function iterates through a 2D array of file descriptors, where each
 * sub-array represents a pipe, and attempts to close them using the
 * `close_pipe` function. If any call to `close_pipe` fails, the program
 * terminates with an exit status of 1.
 *
 * @param pipes A 2D array of integers where each sub-array contains two file
 *              descriptors representing a pipe.
 *
 */
void	close_pipes(int pipes[][2]);

/**
 * @brief Executes a command based on its position in a pipeline.
 *
 * This function determines whether the command to be executed is the first,
 * a middle, or the last command in a pipeline, and calls the appropriate
 * helper function to handle the execution.
 *
 * @param argv Array of command-line arguments. Each command and its arguments
 *             are stored in this array.
 * @param i The index of the current command in the pipeline.
 * @param pipes A 2D array representing the file descriptors for the pipes
 *              used in the pipeline. Each row contains two file descriptors:
 *              one for reading and one for writing.
 * @param paths An array of possible paths where the command binaries may be
 *              located. This is used to resolve the command's executable path.
 */
void	execute_cmd(char *argv[], size_t *i, int pipes[][2], char **paths);

/**
 * @brief Finds the full path of a command in the given paths.
 *
 *
 * This function splits the given command into arguments, appends a '/' to the
 * command name, and iterates through the provided paths to construct the full
 * command path. It checks if the constructed path is executable using `access`.
 * If the command is found, the function returns the full path. If not, it
 * frees allocated memory, prints an error message, and returns NULL.
 *
 * @param paths An array of strings representing the directories to search in.
 * @param command The command to search for (e.g., "ls").
 *
 * @return
 * - A dynamically allocated string containing the full path to the command,
 *   if found.
 *
 * - NULL if the command is not found or an error occurs.
 *
 * @note
 * - The caller is responsible for freeing the returned string.
 *
 * - This function frees the `paths` array and its contents if the command is
 *   not found and exits the program.
 */
char	*get_cmd_path(char command[], char **paths);

/**
 * @brief Reads input from the standard input until a specified EOF marker
 *        is encountered, dynamically allocating memory as needed.
 *
 *
 * This function reads input from the standard input (STDIN_FILENO) in chunks
 * and appends it to a dynamically allocated buffer. It stops reading when the
 * EOF marker is encountered or an error occurs. The buffer is resized
 * dynamically if it becomes full during reading. The function ensures proper
 * handling of memory allocation and deallocation.
 *
 * @param eof The string that marks the end of input (EOF). It must start and
 *            end by newline (\\n).
 * @param eof_size The size of the EOF string, including the newlines.
 *
 * @return A pointer to the dynamically allocated buffer containing the input
 *         read up to the EOF marker.
 * @note: The caller is responsible for freeing this memory.
 */
char	*heredoc(char *eof, size_t eof_size);

/**
 * @brief Doubles the size of the heredoc buffer.
 *
 * Reallocates memory for the buffer to double its size. Exits
 * the program with EXIT_FAILURE if reallocation fails.
 *
 * @param buffer_size Pointer to the current buffer size, updated
 *                    to the new size after reallocation.
 * @param buffer Pointer to the buffer, updated to point to the
 *
 *
 * @note Exits the program with EXIT_FAILURE if memory reallocation fails.
 */
void	heredoc_buffer_realloc(size_t *buffer_size, char **buffer);

/**
 * @brief Initializes the heredoc buffer and related variables.
 *
 * Sets up variables for handling heredoc input. Allocates memory for the
 * buffer, initializes buffer stats, and prepares for reading input.
 *
 * @param buffer_size_total Array where buffer_size_total[0] is the allocated
 *                          buffer size and buffer_size_total[1] is the total
 *                          data size in the buffer.
 * @param keep_reading Pointer to a char to control reading (set to 1).
 * @param buffer Pointer to a char pointer for the allocated buffer.
 *
 * @note On allocation failure, calls `ft_perror` and exits with failure.
 */
void	heredoc_init(size_t buffer_size_total[2], char *keep_reading,
			char **buffer);

/**
 * @brief Checks if there was an error reading the heredoc.
 *
 * This function checks the result of a read operation on a heredoc and takes
 * appropriate action based on the number of bytes read. If an error occurs
 * during the read operation, it calls `ft_errfree` to handle the error and
 * terminate the program. If the end of the input is reached (bytes_read == 0),
 * it sets the `keep_reading` flag to 0 to signal that no further reading
 * should occur.
 *
 * @param bytes_read The number of bytes read from the heredoc. A value of -1
 *                   indicates an error, and 0 indicates the end of input.
 * @param keep_reading A pointer to a flag that determines whether to continue
 *                     reading. This is set to 0 if the end of input is reached.
 * @param buffer A pointer to the buffer used for reading. This is passed to
 *               `ft_errfree` in case of an error for proper cleanup.
 */
void	heredoc_no_read(ssize_t bytes_read, char *keep_reading, char **buffer);

/**
 * @brief Opens the specified file in read-only mode and sets it as stdin.
 *
 * Opens the file specified by `file` in read-only mode. If the file cannot
 * be opened, it prints an error message and terminates the program. On
 * success, redirects stdin to read from the file and closes the original
 * file descriptor.
 *
 * @param file Path to the file to be opened and set as stdin.
 * @note It closes the program if an error occurs
 */
void	set_infile(char file[]);

/**
 * @brief Opens or creates an output file and redirects the standard output
 *        to it.
 *
 * This function handles the creation or opening of an output file, with the
 * option to either append to the file or truncate it. Once the file is opened,
 * the standard output (STDOUT_FILENO) is redirected to the file descriptor.
 *
 * @param file The path to the output file.
 * @param append A flag indicating whether to append to the file or truncate it:
 *
 *               - If non-zero, the file is opened in append mode.
 *
 *               - If zero, the file is opened in truncate mode.
 *
 * @note The function will terminate the program with an error message if the
 *       file cannot be opened.
 */
void	set_outfile(char file[], char append);

/**
 * @brief Waits for all child processes to terminate.
 *
 * This function uses a loop to call `waitpid` repeatedly, waiting for any
 * child process to terminate. It continues until there are no more child
 * processes to wait for. If an error occurs during the wait process and
 * the error is not `ECHILD` (indicating there are no child processes left),
 * it prints an error message using `perror`.
 */
void	wait_childs(void);

#endif
