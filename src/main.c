/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:59:19 by pablo             #+#    #+#             */
/*   Updated: 2025/04/03 22:24:56 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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

char	*heredoc(char *eof, size_t eof_size)
{
	char	*buffer;
	char	*line;
	char	*tmp;

	buffer = NULL;
	while (1)
	{
		ft_printf("heredoc >");
		line = ft_get_next_line(STDIN_FILENO);
		if (line)
		{
			if (!ft_strncmp(line, eof, eof_size))
				return (ft_free((void **)&line), buffer);
			tmp = buffer;
			buffer = ft_strjoin(buffer, line);
			if (tmp)
				ft_free((void **)&tmp);
			ft_free((void **)&line);
			if (!buffer)
				return (ft_free((void **)&buffer),
					perror("Error joining heredoc"), NULL);
		}
		else
			return (ft_free((void **)&buffer), perror("Heredoc error"), NULL);
	}
}

static int	heredoc_pipe(char *argv[], int heredoc_pipe[])
{
	char	*buffer;

	buffer = heredoc(argv[2], ft_strlen(argv[2]));
	if (!buffer)
		return (1);
	write(heredoc_pipe[1], buffer, ft_strlen(buffer));
	ft_free((void **)&buffer);
	dup2(heredoc_pipe[0], STDIN_FILENO);
	return (0);
}

int	set_infile(char file[])
{
	int	file_fd;

	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	dup2(file_fd, STDIN_FILENO);
	close(file_fd);
	return (0);
}

int	set_outfile(char file[], char append)
{
	int	file_fd;

	if (append)
		file_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	dup2(file_fd, STDOUT_FILENO);
	close(file_fd);
	return (0);
}

char	*get_cmd_path(char command[], char **paths)
{
	char	*cmd;
	char	**splitted_args;
	char	*cmd_path;
	size_t	i;

	splitted_args = ft_split(command, ' ');
	if (!splitted_args)
		ft_perror("Error splitting arguments from command", ENOMEM, 0);
	i = 0;
	if (!splitted_args[0])
	{
		ft_matrix_free((void **)splitted_args, 0);
		ft_matrix_free((void **)paths, 0);
		ft_perror("Error Empty command", ENODATA, 0);
		return (NULL);
	}
	cmd = ft_strjoin("/", splitted_args[0]);
	if (!cmd)
	{
		ft_matrix_free((void **)paths, 0);
		ft_matrix_free((void **)splitted_args, 0);
		ft_perror("Error adding '/' to command", ENOMEM, 0);
		return (NULL);
	}
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
	return (ft_free((void **)&cmd), ft_matrix_free((void **)splitted_args, 0),
		ft_matrix_free((void **)paths, 0), NULL);
}

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
	dup2(pipes[i - 2][1], STDOUT_FILENO);
	clean_pipes(pipes);
	execve(cmd_path, args, environ);
	ft_free((void **)&cmd_path);
	ft_matrix_free((void **)args, 0);
	ft_perror("Error executing command", 0, EXIT_FAILURE);
}

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
	dup2(pipes[i - 3][0], STDIN_FILENO);
	dup2(pipes[i - 2][1], STDOUT_FILENO);
	clean_pipes(pipes);
	execve(cmd_path, args, environ);
	ft_free((void **)&cmd_path);
	ft_matrix_free((void **)args, 0);
	ft_matrix_free((void **)paths, 0);
	ft_perror("Error executing command", 0, EXIT_FAILURE);
}

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
		dup2(pipes[i - 3][0], STDIN_FILENO);
		clean_pipes(pipes);
		execve(cmd_path, args, environ);
		ft_free((void **)&cmd_path);
		ft_matrix_free((void **)args, 0);
		ft_perror("Error executing command", 0, EXIT_FAILURE);
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

int	fork_loop(int argc, char *argv[], int **pipes)
{
	int		i;
	pid_t	pid;
	char	**paths;

	i = 2;
	paths = ft_split(ft_getenv("PATH"), ':');
	if (!paths)
	{
		clean_pipes(pipes);
		ft_perror("Error getting cmd paths", 0, EXIT_FAILURE);
	}
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		i = -1;
	while (i < argc - 1)
	{
		pid = fork();
		if (pid == 0)
		{
			execute_cmd(argv, i, pipes, paths);
			exit(EXIT_FAILURE);
		}
		else if (pid == -1)
		{
			ft_matrix_free((void **)paths, 0);
			clean_pipes(pipes);
			ft_perror("Error forking", 0, EXIT_FAILURE);
		}
		else if (i == -1)
			i = 3;
		++i;
	}
	clean_pipes(pipes);
	ft_matrix_free((void **)paths, 0);
	return (wait_childs(pid));
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
int	main(int argc, char *argv[])
{
	size_t	n_pipes;
	int		**pipes;

	if (argc < 5)
		ft_perror("Not enough arguments", EINVAL, EXIT_FAILURE);
	n_pipes = argc - 4;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		++n_pipes;
	pipes = ft_calloc(n_pipes + 1, sizeof(int *));
	if (create_pipes(pipes, n_pipes))
		ft_perror("Error creating pipes", 0, EXIT_FAILURE);
	return (fork_loop(argc, argv, pipes));
}
