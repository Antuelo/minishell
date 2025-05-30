/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:19:19 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/30 22:06:32 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_redirections(t_cmd *cmd, t_exec *exec)
{
	if (exec->fd_in != 0)
	{
		dup2(exec->fd_in, STDIN_FILENO);
		close(exec->fd_in);
	}
	if (cmd->next)
	{
		dup2(exec->pipe_fd[1], STDOUT_FILENO);
		close(exec->pipe_fd[0]);
		close(exec->pipe_fd[1]);
	}
}

void	parent_process(t_exec *exec, t_cmd *cmd, int i)
{
	waitpid(exec->pid[i], NULL, 0);
	if (exec->fd_in != 0)
		close(exec->fd_in);
	if (cmd->next)
	{
		close(exec->pipe_fd[1]);
		exec->fd_in = exec->pipe_fd[0];
	}
}

/*if (cmd->outfile != NULL && cmd->append != -1)	si parsing ">" or ">>"
** if (cmd->infile)									si parsing "<"*/
void	execute_fork(t_cmd *cmd, t_exec *exec, char **envp, int i)
{
	char	*fullpath;
	int		id_builtin;

	id_builtin = is_builtin(cmd->args[0]);
	if (exec->pid[i] == 0)
	{
		setup_redirections(cmd, exec);
		if (id_builtin <= 0)
		{
			fullpath = get_cmd_path(cmd->args[0], envp);
			controlpath(fullpath, cmd);
		}
		if (cmd->outfile != NULL && cmd->append != -1)
			handle_outfile(cmd);
		if (cmd->infile)
			handle_infile(cmd);
		if (id_builtin >= 1 && id_builtin <= 3)
			exec_builtin(cmd, envp);
		else
			execute_execve(fullpath, cmd, envp);
	}
	else
		parent_process(exec, cmd, i);
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
int	execute_pipeline(t_cmd *cmd_list, char **envp)
{
	t_exec	exec;
	int		i;
	int		control;
	t_cmd	*cmd;

	i = 0;
	cmd = cmd_list;
	if (init_exec(&exec, countcmds(cmd_list)))
		return (1);
	if (control_builtin(cmd))
		return (0);
	while (cmd)
	{
		control = control_fork_pipe(cmd, &exec, i);
		if (control == -1)
			return (free(exec.pid), 1);
		execute_fork(cmd, &exec, envp, i);
		i++;
		cmd = cmd->next;
	}
	wait_all_processes(&exec);
	free(exec.pid);
	return (0);
}
