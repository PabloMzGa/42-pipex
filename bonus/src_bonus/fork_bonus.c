/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:16:10 by pablo             #+#    #+#             */
/*   Updated: 2025/05/22 22:17:56 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Handles the creation of a new process using fork and sets up process
 * information.
 *
 * This function is responsible for forking the current process and updating
 * the provided process information structure (`t_pinfo`). It may also handle
 * additional setup required for the child or parent process, depending on the
 * implementation.
 *
 * @param pinfo Pointer to a t_pinfo structure where process information will
 *              be stored or updated.
 * @param argv  Array of argument strings, typically passed from main().
 * @return The process ID (pid_t) of the child process in the parent, or 0 in
 *         he child. Returns -1 if the fork fails.
 */
static pid_t	handle_fork(t_pinfo *pinfo, char *argv[])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execute_cmd(pinfo, argv);
		exit(EXIT_FAILURE);
	}
	else if (pid == -1)
	{
		clean_pinfo(pinfo);
		ft_perror("Error forking", 0, EXIT_FAILURE);
	}
	return (pid);
}

int	fork_loop(int argc, char *argv[], int **pipes)
{
	int		i;
	pid_t	pid;
	t_pinfo	*pinfo;

	pinfo = set_pinfo(pipes);
	if (!pinfo)
		return (1);
	i = 2;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		i = 3;
		pinfo->heredoc_tmp_file = set_heredoc_tmp_file(argv[2]);
		if (!pinfo->heredoc_tmp_file)
			return (clean_pinfo(pinfo), 1);
	}
	while (i < argc - 1)
	{
		pinfo->i = i;
		pid = handle_fork(pinfo, argv);
		++i;
	}
	return (wait_childs(pid, pinfo));
}
