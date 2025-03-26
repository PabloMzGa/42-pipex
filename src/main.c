/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:34:12 by pablo             #+#    #+#             */
/*   Updated: 2025/03/26 23:06:12 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

char	*get_cmd_path(char command[], char **paths)
{
	char	*cmd;
	char	*cmd_path;
	size_t	i;

	i = 0;
	cmd = ft_strjoin("/", command);
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			free(cmd);
			return (cmd_path);
		}
		free(cmd_path);
		++i;
	}
	return (free(cmd), ft_matrix_free((void **)paths, 0),
		ft_perror("Command not found", 0, 1), NULL);
}

void	check_files(int argc, char *argv[])
{
	if (access(argv[1], F_OK) == -1)
		ft_perror("Error, infile does not exist", 0, 1);
	if (access(argv[1], R_OK) == -1)
		ft_perror("Error, infile can't be read", 0, 1);
	if (access(argv[argc - 1], F_OK) == -1)
		ft_perror("Error, outfile does not exist", 0, 1);
	if (access(argv[argc - 1], W_OK) == -1)
		ft_perror("Error, infile can't be written", 0, 1);
}
void	set_infile(char file[])
{
	int	file_fd;

	file_fd = open(file, O_RDONLY);
	if (file_fd == -1)
		ft_perror("Error opening file", 0, 1);
	dup2(file_fd, STDIN_FILENO);
	close(file_fd);
}

void	set_outfile(char file[])
{
	int	file_fd;

	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC);
	if (file_fd == -1)
		ft_perror("Error opening file", 0, 1);
	dup2(file_fd, STDOUT_FILENO);
	close(file_fd);
}

void	close_pipes(int pipes[][2])
{
	if (close(pipes[0][0]) == -1)
		ft_perror("Error closing pipes[0][0]", 0, 1);
	if (close(pipes[0][1]) == -1)
		ft_perror("Error closing pipes[0][1]", 0, 1);
	if (close(pipes[1][0]) == -1)
		ft_perror("Error closing pipes[1][0]", 0, 1);
	if (close(pipes[1][1]) == -1)
		ft_perror("Error closing pipes[0][1]", 0, 1);
}

/**
 * TODO: Tengo que revisar la lógica del cierre de las pipes en el padre
 * No entiendo bien el porque hay que cerrarlos, y cómo eso afecta
 * a los forks de iteraciones anteriores.acos
 *
 * TODO: El persaeo de los argumentos de los comandos
 */
void	execute_cmd(int argc, char *argv[], size_t i, int pipes[][2],
		char **paths)
{
	extern char	**environ;

	if (i == 2)
	{
		/* 		set_infile(argv[1]);
				dup2(pipes[0][1], STDOUT_FILENO);
				close_pipes(pipes); */
		//execve(get_cmd_path(argv[i], paths), NULL, environ);
		return;
	}
	else if (i == argc - 2)
	{
		/* 		set_outfile(argv[argc - 1]);
				dup2(pipes[!(i % 2)][0], STDIN_FILENO);
				*/
		//execve(get_cmd_path(argv[i], paths), NULL, environ);
		return;
	}
	else
	{
		/* 		dup2(pipes[!(i % 2)][0], STDIN_FILENO);
				dup2(pipes[(i % 2)][1], STDOUT_FILENO);
				close_pipes(pipes); */
		//execve(get_cmd_path(argv[i], paths), NULL, environ);
		return;
	}
}

/**
 * TODO: No se como he hecho un forkbomb. Necesito comprobar que cojones
 * está pasando y por que sólo hace las 2 primeras iteraciones.
 * Parece que es gestion de los fork y del execve
 */
int	main(int argc, char *argv[])
{
	pid_t	pid;
	size_t	i;
	char	**paths;
	int		pipes[2][2];

	if (argc < 5)
		ft_perror("Not enough arguments", EINVAL, 1);
	check_files(argc, argv);
	i = 2;
	paths = ft_split(ft_getenv("PATH"), ':');
	if (pipe(pipes[0]) == -1)
		ft_perror("Error creating the first pipe", 0, 1);
	if (pipe(pipes[1]) == -1)
		ft_perror("Error creating the second pipe", 0, 1);
	while (i < argc - 1)
	{
		pid = fork();
		if (pid == 0)
			execute_cmd(argc, argv, i, pipes, paths);
		else if (pid == -1)
			ft_perror("Error forking", 0, 1);
		else
		{
			waitpid(pid, NULL, 0);
			++i;
			ft_printf("Incrementando i = %i con pid=%i de %i\n", i, pid, argc
				- 1);
		}
	}
}
