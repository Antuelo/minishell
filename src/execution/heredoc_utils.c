/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:55:07 by anoviedo          #+#    #+#             */
/*   Updated: 2025/08/13 17:33:00 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

void	reset_readline(void)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/* tcgetattr me permet de garder l'état orifinal dans "original"
** comme ça, je peux le reprendre plus tard */
int	save_original_terminal_mode(t_termios *original)
{
	if (tcgetattr(STDIN_FILENO, original) == -1)
	{
		perror("minishell: tcgetatrr");
		return (-1);
	}
	return (0);
}

void	restore_original_terminal_mode(t_termios *original)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, original) == -1)
		perror("minishell: tcsetattr");
}

int	handle_heredoc_interrupt(t_cmd *cmd, t_termios *term)
{
	close(cmd->hdoc_pipe[0]);
	restore_original_terminal_mode(term);
	reset_readline();
	return (130);
}

/*	if (WIFEXITED(status) si l'enfant a fini ...
	 et && WEXITSTATUS(status) == 130 si fini avec ctrol + c
*/
int	wait_for_heredoc(pid_t pid, t_cmd *cmd, t_termios *term)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(cmd->hdoc_pipe[0]);
		restore_original_terminal_mode(term);
		write(STDOUT_FILENO, "\n", 1);
		return (130);
	}
	return (0);
}
