/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:29:14 by pablo             #+#    #+#             */
/*   Updated: 2025/05/22 22:09:20 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	clean_pipe(int *pipe_fds)
{
	char	status;

	status = 0;
	if (close(pipe_fds[0]) == -1)
		status = 1;
	if (close(pipe_fds[1]) == -1)
		status = 1;
	ft_free((void **)&pipe_fds);
	if (status)
		ft_perror("Fatal error closing pipes", 0, 0);
}

int	wait_childs(pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		exit_status;

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
	return (exit_status);
}

int	*create_pipe(void)
{
	int	*pipe_fds;

	pipe_fds = malloc(sizeof(int) * 2);
	if (!pipe_fds)
		return (perror("Error allocating pipe"), NULL);
	if (pipe(pipe_fds) == -1)
		return (perror ("Error creating pipe"), free(pipe_fds), NULL);
	return (pipe_fds);
}
