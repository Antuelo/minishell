/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:52:26 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 21:46:06 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	freepath(char **patch)
{
	int	i;

	i = 0;
	while (patch[i])
		free(patch[i++]);
	free(patch);
}

void	execute_path(char *path, char **envp, char **args, char *cmd)
{
	int	i;

	i = 0;
	if (path)
	{
		execve(path, args, envp);
		perror("execve");
		free(path);
		freepath(args);
		exit(1);
	}
	else
	{
		while (cmd[i])
			write(2, &cmd[i++], 1);
		write(2, ": command not found\n", 20);
		freepath(args);
		exit(1);
	}
}

void	run_command(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
	{
		perror("split");
		exit(1);
	}
	path = get_cmd_path(args[0], envp);
	execute_path (path, envp, args, cmd);
}

/*
void	execute_cmdin(t_pipex *px, char **argv, char **envp, int error)
{
	dup2(px->fd_in, STDIN_FILENO);
	dup2(px->pipe_fd[1], STDOUT_FILENO);
	close(px->pipe_fd[1]);
	close(px->fd_in);
	if (error != 0)
		return ;
	run_command(argv[2], envp);
}*/

/**px->pipe_fd[0] lire le pipe */
/*
void	execute_cmdout(t_pipex *px, char **argv, char **envp)
{
	dup2(px->pipe_fd[0], STDIN_FILENO);
	dup2(px->fd_out, STDOUT_FILENO);
	close(px->pipe_fd[0]);
	close(px->fd_out);
	run_command(argv[3], envp);
}*/
