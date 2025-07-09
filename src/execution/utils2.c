/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:10:29 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 22:10:37 by llabatut         ###   ########.ch       */
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

/*dans ce cas 		if (access(cmd, X_OK) == 0)
** X_OK C'est pour savoir si il est executable
*/
char	*control_slash(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	return (NULL);
}

static char	**extract_paths(char **envp)
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
