/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:19:19 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/21 21:54:57 by anoviedo         ###   ########.fr       */
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

void	execute_fork(t_cmd *cmd, t_exec *exec, char **envp, int i)
{
	char	*fullpath;

	if (exec->pid[i] == 0)
	{
		setup_redirections(cmd, exec);
		fullpath = get_cmd_path(cmd->args[0], envp); // obtenir le path
		controlpath(fullpath, cmd);                       // control d'erreur pour path (free aussi)
		if (cmd->outfile != NULL && cmd->append != -1)// si parsing ">" or ">>"
			handle_outfile(cmd);
		if (cmd->infile)							// si parsing "<"
			handle_infile(cmd);
		if (is_builtin(cmd->args[0]) >= 4)
			exec_builtin(cmd, envp);
		else
		{
			execve(fullpath, cmd->args, envp);
			perror("execve");
			free(fullpath);
			free_cmd(cmd);
			exit(1);
		}
	}
	else
		parent_process(exec, cmd, i);
}

int	control_fork_pipe(t_cmd *cmd, t_exec *exec, int i, char **envp)
{
	int	control;

	control = is_builtin(cmd->args[0]);				//pour savoir si c'est un built or proccess fils
	if (cmd->next && pipe(exec->pipe_fd) == -1)		//control "si il existe autre comande (pipe)... et pipe fonctionne"
		return (perror("pipe"), -1);
	if (control >= 1 && control <= 3)
	{
		exec_builtin(cmd, envp);
		return (42);
	}
	else
	{
		exec->pid[i] = fork();
		if (exec->pid[i] < 0)
			return (perror("fork"), 1);
		return (0);
	}
}

int	execute_pipeline(t_cmd *cmd_list, char **envp)
{
	t_exec	exec;
	int		i;
	int		control;
	t_cmd	*cmd;

	i = 0;
	exec.fd_in = 0;
	cmd = cmd_list;										//copier le cmd
	exec.cmd_count = countcmds(cmd_list);				//compter la quantité de cmd
	exec.pid = malloc(sizeof(pid_t) * exec.cmd_count);
	if (!exec.pid)
		return (perror("malloc"), 1);
	while (cmd)											//execution de pipes
	{
		control = control_fork_pipe(cmd, &exec, i, envp);		//génerer fork et pipe
		if (control == -1)
			return (free(exec.pid), 1);
		if (control != 42)								//si c'est un built, on fait pas fork
			execute_fork(cmd, &exec, envp, i);				//executer les forks
		i++;
		cmd = cmd->next;
	}
	wait_all_processes(&exec);
	free(exec.pid);
	return (0);
}
