/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_begin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:45:32 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/07 01:11:08 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/*	control_builtin dans le cas de "non pipes",
	seulement un builtin (echo par ex).
	init_exec initialitation de t_exec*/
static int	prepare_pipeline(t_cmd *cmd_list, t_exec *exec, char ***envp)
{
	t_cmd	*cmd;
	int		status;

	if (control_builtin(cmd_list, envp))
		return (1);
	status = heredoc(cmd_list, 0);
	if (status)
		return (g_exit_status = status, -1);
	cmd = cmd_list;
	while (cmd)
	{
		if (!cmd->invalid)
			break ;
		cmd = cmd->next;
	}
	if (!cmd)
		return (g_exit_status = 1, -1);
	if (init_exec(exec, countcmds(cmd_list)))
		return (-1);
	return (0);
}

/*	return 1 si erreur en pipe, 0 si tout va bien
	j'ai éliminé un pipe de plus: -----> (déjà executé en control_fork_pipes)
		if (cmd->next && pipe(exec->pipe_fd) == -1)
		return (perror("pipe"), free(exec->pid), 1);	*/
static int	handle_invalid_cmd(t_cmd *cmd, t_exec *exec, int i)
{
	if (cmd->next && pipe(exec->pipe_fd) == -1)
		return (perror("pipe"), 1);
	if (cmd->next)
	{
		close(exec->pipe_fd[1]);
		if (exec->fd_in != STDIN_FILENO)
			close(exec->fd_in);
		exec->fd_in = exec->pipe_fd[0];
	}
	exec->pid[i] = -1;
	return (0);
}

static int	handle_valid_cmd(t_cmd *cmd, t_exec *exec, char **envp, int i)
{
	int	ctrl;

	ctrl = control_fork_pipe(cmd, exec, i);
	if (ctrl != 0)
		return (1);
	execute_fork(cmd, exec, envp, i);
	return (0);
}

static int	run_pipeline(t_cmd *cmd_list, t_exec *exec, char ***envp)
{
	t_cmd	*cmd;
	int		i;

	cmd = cmd_list;
	i = 0;
	while (cmd)
	{
		if ((cmd->invalid && handle_invalid_cmd(cmd, exec, i)) || (!cmd->invalid
				&& handle_valid_cmd(cmd, exec, *envp, i)))
			return (1);
		i++;
		cmd = cmd->next;
	}
	wait_all_processes(exec);
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	return (0);
}

/*	prep 1 builtin executés dans le pere
	prep -1 error
	sinon, des children and pipes*/
int	execute_pipeline(t_cmd *cmd_list, char ***envp)
{
	t_exec	exec;
	int		prep;
	int		ret;

	exec.pid = NULL;
	prep = prepare_pipeline(cmd_list, &exec, envp);
	if (prep == 1)
		ret = 0;
	else if (prep == -1)
		ret = 1;
	else
		ret = run_pipeline(cmd_list, &exec, envp);
	return (ret);
}
