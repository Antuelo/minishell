/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:46:10 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/01 22:27:28 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex/pipex.h"
#include "../libft/libft.h"
#include <stdio.h>

char	**extract_paths(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	**path;
	char	*temp;
	char	*full_path;
	int		j;

	path = extract_paths(envp);
	if (!path)
		return (NULL);
	j = 0;
	while (path[j])
	{
		temp = ft_strjoin(path[j], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (freepath(path), full_path);
		free(full_path);
		j++;
	}
	freepath(path);
	return (NULL);
}
