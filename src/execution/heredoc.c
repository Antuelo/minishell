/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:07:36 by anoviedo          #+#    #+#             */
/*   Updated: 2025/08/23 22:04:06 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

#define MENS "minishell: warning: here-document delimited by end-of-file\n"

/*STDERR_FILENO = Standard error output. Expands to: 2*/
static void	heredoc_signal_handler(int signo)
{
	(void)signo;
	write(STDERR_FILENO, "^C", 2);
	close(STDIN_FILENO);
	g_exit_status = 130;
	write(1, "\033[1A", 4);
}

static void	close_pipes(int pipes[2])
{
	if (pipes[0] != -1)
		close(pipes[0]);
	if (pipes[1] != -1)
		close(pipes[1]);
}

/*le coeur du heredoc ... là on fait readline dedié*/
static void	child_heredoc(t_cmd *cmd, char *delim, char ***envp)
{
	char			*line;
	extern int		rl_catch_signals;

	g_exit_status = 0;
	signal(SIGINT, heredoc_signal_handler);
	close(cmd->hdoc_pipe[0]);
	while (1)
	{
		line = readline("> ");
		if (g_exit_status == 130 && !line)
			return (f_heredoc(cmd, line, envp), _exit(130));
		else if (!line)
		{
			f_heredoc(cmd, line, envp);
			return (ft_putstr_fd(MENS, STDERR_FILENO), exit (0));
		}
		if (ft_strcmp(line, delim) == 0)
			return (f_heredoc(cmd, line, envp), _exit(0));
		write(cmd->hdoc_pipe[1], line, ft_strlen(line));
		write(cmd->hdoc_pipe[1], "\n", 1);
		free(line);
	}
}

/*	Je met en place les termios (donc la terminal) et j excute dans un
	fork, les heredoc. En fin, j'attends pour tous les heredoc (comme
	wait_all_proccess) dans wait_for_heredoc*/
static int	execute_heredoc(t_cmd *cmd, char *delim, char ***envp)
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
		child_heredoc(cmd, delim, envp);
	if (cmd->hdoc_pipe[1] != -1)
	{
		close(cmd->hdoc_pipe[1]);
		cmd->hdoc_pipe[1] = -1;
	}
	status = wait_for_heredoc(pid, cmd, &origin_termios);
	signal(SIGINT, handle_signs);
	return (status);
}

int	heredoc(t_cmd *cmd_list, int status, char ***envp)
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
				status = execute_heredoc(cmd, cmd->delimiter[i], envp);
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
