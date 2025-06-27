/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/27 14:19:32 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	init_exec(t_exec *exec, int count)
{
	int	i;

	i = 0;
	exec->cmd_count = count;
	exec->pid = malloc(sizeof(pid_t) * count);
	if (!exec->pid)
		return (perror("malloc"), 1);
	while (i < count)
		exec->pid[i++] = -1;
	exec->fd_in = STDIN_FILENO;
	exec->pipe_fd[0] = -1;
	exec->pipe_fd[1] = -1;
	return (0);
}

int	control_builtin(t_cmd *cmd_list, char ***envp)
{
	int	id;

	id = is_builtin(cmd_list->args[0]);
	if (!cmd_list->next && !cmd_list->infile && !cmd_list->outfile && id > 0)
	{
		if (id == 5)
			ft_exit(cmd_list->args);
		else
			g_exit_status = exec_builtin(cmd_list, envp);
		return (1);
	}
	return (0);
}

int	control_infiles(t_cmd *cmd)
{
	if (cmd->outfile != NULL && cmd->append != -1)
		handle_outfile(cmd);
	if (cmd->infile)
		handle_infile(cmd);
	if (g_exit_status == 1)
		return (1);
	return (0);
}

void	preparing_heredoc(t_cmd *cmd)
{
	char	*line;

	if (pipe(cmd->hdoc_pipe) == -1)
	{
		perror("pipe heredoc");
		return ;
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			perror("here-document need delimited by end-of-file");
			break ;
		}
		if (ft_strncmp(line, cmd->delimiter, ft_strlen(cmd->delimiter)) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, cmd->hdoc_pipe[1]);
		ft_putchar_fd('\n', cmd->hdoc_pipe[1]);
		free(line);
	}
	close(cmd->hdoc_pipe[1]);
}

/*int	preparing_heredoc(t_cmd *cmds)
{
	char	*line;

	while (cmds)
	{
		if (cmds->heredoc)
		{
			if (pipe(cmds->hdoc_pipe) == -1)
				return (perror("pipe"), 1);
			while (1)
			{
				line = readline("> ");
				if (!line || ft_strncmp(line, cmds->delimiter,
						ft_strlen(cmds->delimiter)) == 0)
					break ;
				write(cmds->hdoc_pipe[1], line, ft_strlen(line));
				write(cmds->hdoc_pipe[1], "\n", 1);
				free(line);
			}
			free(line);
			close(cmds->hdoc_pipe[1]);
		}
		cmds = cmds->next;
	}
	return (0);
}*/
