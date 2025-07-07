/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:07:36 by anoviedo          #+#    #+#             */
/*   Updated: 2025/07/07 13:30:14 by anoviedo         ###   ########.fr       */
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

static void	child_heredoc(t_cmd *cmd, char *delim)
{
	char	*line;

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
			free(line);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
			break ;
		write(cmd->hdoc_pipe[1], line, ft_strlen(line));
		write(cmd->hdoc_pipe[1], "\n", 1);
		free(line);
	}
	close(cmd->hdoc_pipe[1]);
	free(line);
	exit(0);
}

static int	execute_heredoc(t_cmd *cmd, char *delim)
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
		child_heredoc(cmd, delim);
	close(cmd->hdoc_pipe[1]);
	status = wait_for_heredoc(pid, cmd, &origin_termios);
	signal(SIGINT, handle_signs);
	return (status);
}

int	heredoc(t_cmd *cmd_list, int status)
{
	t_cmd	*cmd;
	int		i;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->heredoc)
		{
			if (!cmd->delimiter || !cmd->delimiter[0])
				return (2);
			i = 0;
			while (cmd->delimiter[i])
			{
				status = execute_heredoc(cmd, cmd->delimiter[i]);
				if (status)
					return (close(cmd->hdoc_pipe[0]), status);
				if (cmd->delimiter[i + 1])
					close(cmd->hdoc_pipe[0]);
				i++;
			}
		}
		cmd = cmd->next;
	}
	return (status);
}
