/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:07:36 by anoviedo          #+#    #+#             */
/*   Updated: 2025/07/01 19:12:24 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	heredoc_signal_handler(int signo)
{
	(void)signo;
	write(STDERR_FILENO, "^C", 2);
	exit(130);
}

static void	close_pipes(int pipes[2])
{
	if (pipes[0] != -1)
		close(pipes[0]);
	if (pipes[1] != -1)
		close(pipes[1]);
}

static void	child_heredoc(t_cmd *cmd)
{
	char	*line;

	line = NULL;
	signal(SIGINT, heredoc_signal_handler);
	close(cmd->hdoc_pipe[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("\
				minishell: warning: here-document delimited by end-of-file\n",
				STDERR_FILENO);
			break ;
		}
		if (ft_strcmp(line, cmd->delimiter) == 0)
			break ;
		write(cmd->hdoc_pipe[1], line, ft_strlen(line));
		write(cmd->hdoc_pipe[1], "\n", 1);
		free(line);
	}
	close(cmd->hdoc_pipe[1]);
	free(line);
	exit(0);
}

static int	execute_heredoc(t_cmd *cmd)
{
	int			pid;
	int			status;
	t_termios	origin_termios;

	if (save_original_terminal_mode(&origin_termios) == -1)
		return (1);
	if (pipe(cmd->hdoc_pipe) == -1)
		return (perror("pipe - heredoc"), 1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (close_pipes(cmd->hdoc_pipe), perror("fork - heredoc"), 1);
	if (pid == 0)
		child_heredoc(cmd);
	close(cmd->hdoc_pipe[1]);
	status = wait_for_heredoc(pid, cmd, &origin_termios);
	signal(SIGINT, handle_signs);
	return (status);
}

int	heredoc(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	int		status;

	cmd = cmd_list;
	status = 0;
	while (cmd)
	{
		if (cmd->heredoc)
		{
			if (!cmd->delimiter)
				return (2);
			else
				status = execute_heredoc(cmd);
			return (status);
		}
		cmd = cmd->next;
	}
	return (0);
}
