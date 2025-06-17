/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:19:19 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/17 11:06:55 by anoviedo         ###   ########.fr       */
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
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (cmd->next)
	{
		close(exec->pipe_fd[1]);
		exec->fd_in = exec->pipe_fd[0];
	}
}

/*	if (cmd->outfile != NULL && cmd->append != -1)		si parsing ">" or ">>"
**	if (cmd->infile)									si parsing "<"*/
void	execute_fork(t_cmd *cmd, t_exec *exec, char **envp, int i)
{
	char	*fullpath;
	int		id_builtin;

	id_builtin = is_builtin(cmd->args[0]);
	if (exec->pid[i] == 0)
	{
		setup_redirections(cmd, exec);
		if (id_builtin == 0)
		{
			fullpath = get_cmd_path(cmd->args[0], envp);
			controlpath(fullpath, cmd);
		}
		if (cmd->outfile != NULL && cmd->append != -1)
			handle_outfile(cmd);
		if (cmd->infile)
			handle_infile(cmd);
		if (id_builtin >= 1 && id_builtin <= 3)
			exit (exec_builtin(cmd, &envp));
		else
			execute_execve(fullpath, cmd, envp);
		exit (1);
	}
	else
		parent_process(exec, cmd);
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

/*fait --- >
** initialiser exec (le struct)
** controler si c'est un builtin hors le procesus fils exemple:
** comme exit, unset, export et cd (parce que ils mofifient envp)
** génerer fork et pipe (control_for_pipe)
** executer les forks (tous les fils avec execute_fork)
** wait_processes attends tout les procces pour eviter les bugs*/
int	execute_pipeline(t_cmd *cmd_list, char ***envp)
{
	t_exec	exec;
	int		i;
	int		control;
	t_cmd	*cmd;

	i = 0;
	cmd = cmd_list;
	if (control_builtin(cmd, envp))
		return (0);
	if (init_exec(&exec, countcmds(cmd_list)))
		return (1);
	while (cmd)
	{
		control = control_fork_pipe(cmd, &exec, i);
		if (control == -1)
			return (free(exec.pid), 1);
		execute_fork(cmd, &exec, (*envp), i);
		i++;
		cmd = cmd->next;
	}
	wait_all_processes(&exec);
	if (exec.fd_in != STDIN_FILENO)
		close(exec.fd_in);
	free(exec.pid);
	return (0);
}
