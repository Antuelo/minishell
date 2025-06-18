/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:52:26 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/18 23:40:20 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

void	handle_infile(t_cmd *cmd)
{
	int	fd_in;

	fd_in = open(cmd->infile, O_RDONLY);
	if (fd_in < 0)
	{
		perror("open infile");
		g_exit_status = 1;
		exit(1);
	}
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

void	handle_outfile(t_cmd *cmd)
{
	int	fd_out;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (cmd->append == 1)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	if (cmd->outfile)
	{
		fd_out = open(cmd->outfile, flags, 0644);
		if (fd_out < 0)
		{
			perror("open outfile");
			g_exit_status = 1;
			exit(1);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

char	*control_slash(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	**path;
	char	*temp;
	char	*full_path;
	int		j;

	temp = control_slash(cmd);
	if (temp)
		return (temp);
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
