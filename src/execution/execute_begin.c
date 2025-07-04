/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_begin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:05:04 by anoviedo          #+#    #+#             */
/*   Updated: 2025/07/04 21:11:07 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	prepare_pipeline(t_cmd *cmd_list, char ***envp, t_exec *exec)
{
	int	status;

	status = heredoc(cmd_list, 0);
	if (status)
		return (g_exit_status = status, -1);
	if (control_builtin(cmd_list, envp))
		return (1);
	if (init_exec(exec, countcmds(cmd_list)))
		return (-1);
	return (0);
}

static int	run_pipeline(t_cmd *cmd_list, t_exec *exec, char ***envp)
{
	t_cmd	*cmd;
	int		i;
	int		ctrl;

	cmd = cmd_list;
	i = 0;
	while (cmd)
	{
		ctrl = control_fork_pipe(cmd, exec, i);
		if (ctrl == -1)
			return (free(exec->pid), 1);
		execute_fork(cmd, exec, *envp, i);
		i++;
		cmd = cmd->next;
	}
	wait_all_processes(exec);
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	free(exec->pid);
	return (0);
}

int	execute_pipeline(t_cmd *cmd_list, char ***envp)
{
	t_exec	exec;
	int		prep;

	prep = prepare_pipeline(cmd_list, envp, &exec);
	if (prep == 1)
		return (0);
	if (prep == -1)
		return (1);
	return (run_pipeline(cmd_list, &exec, envp));
}

/*
** initialiser exec (le struct)
** controler si c'est un builtin hors le procesus fils exemple:
** comme exit, unset, export et cd (parce que ils mofifient envp)
** génerer fork et pipe (control_for_pipe)
** executer les forks (tous les fils avec execute_fork)
** wait_processes attends tout les procces pour eviter les bugs
int	execute_pipeline(t_cmd *cmd_list, char ***envp)
{
	t_exec	exec;
	t_cmd	*cmd;
	int		i;
	int		control;
	int		status;

	i = 0;
	cmd = cmd_list;
	status = heredoc(cmd_list);
	if (status)
		return (g_exit_status = status);
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
}*/
