/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pinfo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:10:22 by pabmart2          #+#    #+#             */
/*   Updated: 2025/05/22 22:18:08 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	clean_pinfo(t_pinfo *pinfo)
{
	if (pinfo->paths)
		ft_matrix_free((void **)(pinfo->paths), 0);
	if (pinfo->pipes)
		clean_pipes(pinfo->pipes);
	if (pinfo->heredoc_tmp_file)
		ft_free((void **)(&pinfo->heredoc_tmp_file));
	ft_free((void **)&pinfo);
}

t_pinfo	*set_pinfo(int **pipes)
{
	t_pinfo	*pinfo;
	char	**paths;

	paths = ft_split(ft_getenv("PATH"), ':');
	if (!paths)
	{
		clean_pipes(pipes);
		ft_perror("Error getting cmd paths", 0, EXIT_FAILURE);
	}
	pinfo = malloc(sizeof(t_pinfo));
	if (!pinfo)
		return (NULL);
	pinfo->paths = paths;
	pinfo->pipes = pipes;
	pinfo->heredoc_tmp_file = NULL;
	return (pinfo);
}
