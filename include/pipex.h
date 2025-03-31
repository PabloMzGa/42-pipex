/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:33:49 by pablo             #+#    #+#             */
/*   Updated: 2025/03/31 23:19:21 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

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

char	*heredoc(char *eof, size_t eof_size);


void	set_infile(char file[],int pipes[][2]);

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

int		wait_childs(pid_t last_pid);

#endif
