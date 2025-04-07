/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_resolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 12:50:33 by pablo             #+#    #+#             */
/*   Updated: 2025/04/07 13:21:57 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Handles errors related to resolving the command path.
 *
 * This function is responsible for freeing allocated memory for the
 * command arguments and paths, and then printing an error message
 * before returning NULL.
 *
 * @param msg The error message to be displayed.
 * @param e The error code or character to be included in the error message.
 * @param splitted_args A null-terminated array of strings representing the
 *        split arguments of the command. This array and its contents are
 *        freed.
 * @param paths A null-terminated array of strings representing possible
 *        paths for the command. This array and its contents are freed.
 * @return Always returns NULL to indicate an error.
 */
static void	*cmd_path_error(char *msg, char e, char **splitted_args,
		char **paths)
{
	ft_matrix_free((void **)splitted_args, 0);
	ft_matrix_free((void **)paths, 0);
	ft_perror(msg, e, 0);
	return (NULL);
}

/**
 * @brief Cleans up resources and handles errors during command resolution.
 *
 * Frees allocated memory and returns NULL in case of an error during the
 * command resolution process. Ensures all dynamically allocated resources
 * are properly released to prevent memory leaks.
 *
 * @param cmd The command to be cleaned up. This is cast to a void pointer
 *        and freed.
 * @param splitted_args A null-terminated array of strings representing the
 *        split arguments of the command. This array and its contents are
 *        freed.
 * @param paths A null-terminated array of strings representing possible
 *        paths for the command. This array and its contents are freed.
 * @return Always returns NULL to indicate an error.
 */
static void	*cmd_error_cleanup(char *cmd, char **splitted_args, char **paths)
{
	ft_free((void **)&cmd);
	ft_matrix_free((void **)splitted_args, 0);
	ft_matrix_free((void **)paths, 0);
	return (NULL);
}

/**
 * @brief Searches for the executable path of a given command in the provided
 * paths.
 *
 * This function iterates through an array of paths, appending the command name
 * to each path and checking if the resulting path is executable. If an
 * executable path is found, it frees the allocated memory for the command and
 * its arguments, and returns the valid executable path. If no valid path is
 * found, it performs cleanup and returns an error.
 *
 * @param paths An array of strings representing the directories to search for
 *              the command.
 * @param cmd A string containing the command name to search for.
 * @param splitted_args An array of strings representing the arguments of the
 *                      command, which will be freed if a valid path is found.
 *
 * @return A string containing the full path to the executable if found, or
 *         NULL if not. In case of failure, it performs cleanup and handles
 *         errors appropriately.
 */
static char	*search_path(char **paths, char *cmd, char **splitted_args)
{
	char	*cmd_path;
	size_t	i;

	i = 0;
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
	return (cmd_error_cleanup(cmd, splitted_args, paths));
}

char	*get_cmd_path(char command[], char **paths)
{
	char	*cmd;
	char	**splitted_args;

	splitted_args = ft_split(command, ' ');
	if (!splitted_args)
		ft_perror("Error splitting arguments from command", ENOMEM, 0);
	if (!splitted_args[0])
		return (cmd_path_error("Error Empty command", ENODATA, splitted_args,
				paths));
	cmd = ft_strjoin("/", splitted_args[0]);
	if (!cmd)
		return (cmd_path_error("Error adding '/' to command", 0, splitted_args,
				paths));
	return (search_path(paths, cmd, splitted_args));
}
