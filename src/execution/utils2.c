/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:52:26 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/07 11:23:48 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include "parsing.h"

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
