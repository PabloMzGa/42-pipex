/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:33:49 by pablo             #+#    #+#             */
/*   Updated: 2025/05/26 19:28:59 by pabmart2         ###   ########.fr       */
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
 * @brief Closes both ends of a pipe and frees the associated memory
 *
 * This function closes both file descriptors in a pipe array and then frees
 * the memory allocated for the pipe. If any close operation fails, an error
 * message is displayed.
 *
 * @param pipe_fds Pointer to an array containing pipe file descriptors
 *                 [0] is the read end and [1] is the write end
 *
 * @note The function will attempt to close both pipe ends even if one fails
 * @note The pipe_fds memory is freed regardless of close operation status
 */
void	clean_pipe(int *pipe_fds);

/**
 * @brief Creates a new pipe and allocates memory for its file descriptors
 *
 * This function allocates memory for two integers to store the read and write
 * file descriptors of a pipe, then initializes the pipe using the system
 * pipe() call.
 *
 * @return A pointer to an array of two integers containing the pipe file
 *         descriptors [0] for reading, [1] for writing, or NULL if an error
 *         occurred
 * @note The caller is responsible for freeing the returned memory when no
 *       longer needed
 * @note In case of failure, appropriate error messages are printed to stderr
 */
int		*create_pipe(void);

/**
 * @brief Executes either the first or last command in a pipeline
 *
 * This function determines which command execution function to call based on
 * the value of i. If i is 2, it executes the first command in the pipeline.
 * Otherwise, it executes the last command.
 *
 * @param argv Array of command line arguments
 * @param paths Array of possible paths to search for executables
 * @param pipe_fds Array of pipe file descriptors for input/output redirection
 * @param i Index determining which command to execute (2 for first, other for
 *          last)
 */
void	execute_cmd(char *argv[], char **paths, int *pipe_fds, int i);

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
 * @brief Creates and manages child processes to execute commands in a pipeline
 *
 * This function iterates through the command arguments, creating a child
 * process for each command using handle_fork(). It first extracts and splits
 * the PATH environment variable to locate executables. After creating all
 * needed child processes, it cleans up resources and waits for all child
 * processes to complete.
 *
 * @param argc Number of command-line arguments
 * @param argv Array of command-line arguments where commands start at index 2
 * @param pipe_fds Array of pipe file descriptors for inter-process
 *                 communication
 *
 * @return Returns the status from wait_childs() which handles process
 *         completion
 * @note The function assumes commands start at argv[2] and continue until
 *       argv[argc-2]
 */
int		fork_loop(int argc, char *argv[], int *pipe_fds);

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
 * @brief Sets up a file as the standard output
 *
 * This function opens the specified file for writing (creating it if it doesn't
 * exist or truncating it if it does), and redirects the standard output to
 * write to this file by duplicating the file descriptor.
 *
 * @param file Path to the output file
 * @return 0 on success, 1 on failure with error message printed to stderr
 */
int		set_outfile(char file[]);

/**
 * @brief Waits for all child processes to terminate and collects the exit
 *        status of a specific child process.
 *
 * This function waits for all child processes to finish their execution,
 * but only returns the exit status of the child process identified by
 * last_pid. If the specified process terminated normally, its exit code
 * is returned. If it terminated due to a signal, the signal value is
 * returned.
 *
 * @param last_pid The PID of the child process whose exit status should be
 *                 returned
 *
 * @return The exit status of the child process specified by last_pid,
 *         or 0 if the specified process was not found
 *
 * @note Prints an error message if waitpid fails for any reason other
 *       than having no more children to wait for (ECHILD).
 */
int		wait_childs(pid_t last_pid);

#endif
