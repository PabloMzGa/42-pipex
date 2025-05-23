/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:33:49 by pablo             #+#    #+#             */
/*   Updated: 2025/05/22 21:27:49 by pablo            ###   ########.fr       */
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
 * @struct s_pipex_info
 * @brief Structure to store information required for pipex execution.
 *
 * @param i
 * Index or counter used during pipex operations.
 *
 * @param pipes
 * 2D array of integers representing file descriptors for pipes.
 *
 * @param paths
 * Array of strings containing possible executable paths.
 *
 * @param heredoc_tmp_file
 * Temporary file name for heredoc input.
 */
typedef struct s_pipex_info
{
	int		i;
	int		**pipes;
	char	**paths;
	char	*heredoc_tmp_file;
}			t_pinfo;

void		clean_pinfo(t_pinfo *pinfo);

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
 *       If any error occurs while closing the pipes, the program will show
 *       an error message.
 */
void		clean_pipes(int **pipes);

/**
 * @brief Creates a specified number of pipes and allocates memory for them.
 *
 * Allocates memory for an array of pipes and initializes each pipe using the
 * `pipe()` system call. If memory allocation or pipe creation fails, it cleans
 * up any previously allocated resources and returns an error.
 *
 * @param pipes A pointer to an array of integer pointers where each element
 *        represents a pipe (an array of two integers for read/write ends).
 * @param n_pipes The number of pipes to create.
 *
 * @return 0 on success, 1 on failure. On failure, an error message is printed
 *         using `perror()` and previously allocated resources are freed.
 *
 * @note The caller must free the allocated memory for the pipes using the
 *       `clean_pipes()` function after use.
 */
int			create_pipes(int **pipes, size_t n_pipes);

/**
 * @brief Executes a command based on its position in a pipeline.
 *
 * Determines whether the command is the first, middle, or last in a sequence
 * of piped commands and calls the appropriate execution function. Handles
 * cleanup of allocated resources such as paths and pipes.
 *
 * @param pinfo Pointer to a t_pinfo structure where process information will
 *              be stored or updated.
 * @param argv Array of command-line arguments.
 * @note Assumes that pipes and paths are already set up. Frees paths and
 *       cleans up pipes after execution.
 */
void		execute_cmd(t_pinfo *pinfo, char *argv[]);

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
char		*get_cmd_path(char command[], char **paths);

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
int			fork_loop(int argc, char *argv[], int **pipes);

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
char		*heredoc(char *eof, size_t eof_size);

/**
 * @brief Removes the temporary file created for heredoc
 *
 * This function deletes the specified file using unlink() system call.
 * If the file deletion fails, an error message is printed to stderr.
 *
 * @param filename The path to the temporary file to be removed
 * @return None
 */
void		remove_heredoc_tmp_file(char *filename);

/**
* @brief Creates a temporary file containing heredoc input.
*
* Reads input from the user until the specified EOF delimiter is encountered,
* writes the input to a newly generated temporary file,
*	and returns the name
* of the file. Handles memory allocation, file creation,
*	and error reporting.
*
* @param eof The end-of-file delimiter string for the heredoc.
* @return On success, returns the name of the temporary file (char *).
*         On failure,
*	returns NULL or 1 depending on the error encountered.
*/
char		*set_heredoc_tmp_file(char *eof);

/**
 * @brief Creates and initializes a pinfo structure
 *
 * This function obtains the PATH environment variable, splits it by colons,
 * and stores it in a newly allocated t_pinfo structure along with the pipes.
 * The heredoc_tmp_file field is initialized to NULL.
 *
 * @param pipes A pointer to an array of pipes to be stored in the structure
 * @return A pointer to the initialized t_pinfo structure, or NULL if memory
 *         allocation fails. If PATH cannot be split, the function will exit
 *         with failure after cleaning the pipes.
 */
t_pinfo		*set_pinfo(int **pipes);

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
int			set_infile(char file[]);

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
int			set_outfile(char file[], char append);

/**
 * @brief Waits for all child processes to terminate and cleans up resources
 *
 * This function waits for all child processes to terminate, captures the exit
 * status of the specified last child process, and performs cleanup operations:
 * - Closes and frees all pipes
 * - Removes any temporary heredoc file
 * - Cleans up process information
 *
 * @param last_pid PID of the specific child process whose exit status to return
 * @param pinfo Pointer to the process information structure containing pipes
 *        and resources
 *
 * @return The exit status of the specified last child process
 */
int			wait_childs(pid_t last_pid, t_pinfo *pinfo);

#endif
