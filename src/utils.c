/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabmart2 <pabmart2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:29:14 by pablo             #+#    #+#             */
/*   Updated: 2025/05/15 18:59:26 by pabmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clean_pipes(int **pipes)
{
	size_t	i;
	char	status;

	i = 0;
	status = 0;
	while (pipes[i])
	{
		if (close(pipes[i][0]) == -1)
			status = 1;
		if (close(pipes[i][1]) == -1)
			status = 1;
		ft_free((void **)&pipes[i]);
		++i;
	}
	ft_free((void **)&pipes[i]);
	ft_free((void **)&pipes);
	if (status)
		ft_perror("Fatal error closing pipes", 0, 1);
}

int	create_pipes(int **pipes, size_t n_pipes)
{
	size_t	i;

	i = 0;
	while (i < n_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			perror("Error allocating pipe");
			clean_pipes(pipes);
			return (1);
		}
		if (pipe(pipes[i]) == -1)
		{
			perror("Error creating pipe");
			clean_pipes(pipes);
			return (1);
		}
		++i;
	}
	return (0);
}

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
int	wait_childs(pid_t last_pid, t_pinfo *pinfo)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	clean_pipes(pinfo->pipes);
	pinfo->pipes = NULL;
	exit_status = 0;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_status = WEXITSTATUS(status);
		}
		pid = waitpid(-1, &status, 0);
	}
	if (pid == -1 && errno != ECHILD)
		perror("Error al esperar a los procesos hijos");
	if (pinfo->heredoc_tmp_file)
		remove_heredoc_tmp_file(pinfo->heredoc_tmp_file);
	clean_pinfo(pinfo);
	return (exit_status);
}
