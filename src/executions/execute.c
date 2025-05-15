/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/16 01:05:02 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	controlpath(char *path)
{
	if (!path)
	{
		perror("error command not found");
		exit(1);
	}
}

void	handle_infile(t_cmd *cmd)
{
	int	fd_in;

	fd_in = open(cmd->infile, O_RDONLY);
	if (fd_in < 0)
	{
		perror("open infile");
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
			exit(1);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

int	execute(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	char	*fullpath;

	pid = fork();
	if (pid == 0)
	{
		fullpath = get_cmd_path(cmd->args[0], envp); // obtenir le path
		controlpath(fullpath);                       // control d'erreru
		if (cmd->outfile != NULL && cmd->append != -1)
			handle_outfile(cmd); // si parsing ">" or ">>"
		if (cmd->infile)
			handle_infile(cmd);
		execve(fullpath, cmd->args, envp);
		perror("execve");
		freepath(fullpath);
		free_cmd(cmd);
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}
