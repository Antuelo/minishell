/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:19:19 by anoviedo          #+#    #+#             */
/*   Updated: 2025/07/05 00:06:32 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/*	ça assure que on ferme le dernier dans un processus fils fd
	if (!cmd->next)
		close(exec->pipe_fd[0]);
	1) Si heredamos un fd distinto de STDIN, redirígelo a STDIN_FILENO
	2) Si hay un siguiente comando, redirige STDOUT al pipe
	3) Cierra siempre el extremo de lectura del pipe en el hijo
*/
void	setup_redirections(t_cmd *cmd, t_exec *exec)
{
	if (exec->fd_in != STDIN_FILENO)
	{
		if (dup2(exec->fd_in, STDIN_FILENO) == -1)
			perror("dup2 stdin");
		close(exec->fd_in);
	}
	if (cmd->next && exec->pipe_fd[1] != -1)
	{
		if (dup2(exec->pipe_fd[1], STDOUT_FILENO) == -1)
			perror("dup2 stdout");
		if (exec->pipe_fd[0] != -1)
			close(exec->pipe_fd[0]);
		if (exec->pipe_fd[1] != -1)
			close(exec->pipe_fd[1]);
	}
}

void	parent_process(t_exec *exec, t_cmd *cmd)
{
	if (cmd->heredoc)
		close(cmd->hdoc_pipe[0]);
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (cmd->next)
	{
		close(exec->pipe_fd[1]);
		exec->fd_in = exec->pipe_fd[0];
	}
}

/*control pour savoir si c'est un built or proccess fils
** control "si il existe autre comande (pipe)... et pipe fonctionne" */
int	control_fork_pipe(t_cmd *cmd, t_exec *exec, int i)
{
	if (cmd->next && pipe(exec->pipe_fd) == -1)
		return (perror("pipe"), -1);
	exec->pid[i] = fork();
	if (exec->pid[i] < 0)
		return (perror("fork"), 1);
	return (0);
}
