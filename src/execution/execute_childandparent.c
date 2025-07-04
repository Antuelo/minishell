/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_childandparent.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:29:40 by anoviedo          #+#    #+#             */
/*   Updated: 2025/07/04 22:31:37 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	condition(t_cmd *cmd, char **envp, int id_builtin, char *fullpath)
{
	if ((id_builtin >= 1 && id_builtin <= 3) || id_builtin == 5)
	{
		if (id_builtin == 5)
			ft_exit(cmd->args);
		exit(exec_builtin(cmd, &envp));
	}
	else
		execute_execve(fullpath, cmd, envp);
}

static char	*control_path(t_cmd *cmd, char **envp, int id_builtin)
{
	char	*fullpath;

	fullpath = NULL;
	if (id_builtin == 0)
	{
		fullpath = get_cmd_path(cmd->args[0], envp);
		controlpath(fullpath, cmd);
		if (g_exit_status == 127)
			exit(127);
	}
	return (fullpath);
}

static void	control_heredoc(t_cmd *cmd)
{
	if (cmd->heredoc)
	{
		dup2(cmd->hdoc_pipe[0], STDIN_FILENO);
		close(cmd->hdoc_pipe[0]);
	}
}

/*	if (cmd->outfile != NULL && cmd->append != -1)		si parsing ">" or ">>"
**	if (cmd->infile)									si parsing "<"
**	signal(SIGINT, SIG_DFL); pour tuer le processus avec ctrl + c
*/
void	execute_fork(t_cmd *cmd, t_exec *exec, char **envp, int i)
{
	char	*fullpath;
	int		id_builtin;

	fullpath = NULL;
	id_builtin = is_builtin(cmd->args[0]);
	if (exec->pid[i] == 0)
	{
		g_exit_status = 0;
		control_heredoc(cmd);
		setup_redirections(cmd, exec);
		if (control_infiles(cmd))
			exit(1);
		else if (!cmd->args || !cmd->args[0])
			exit(0);
		fullpath = (control_path(cmd, envp, id_builtin));
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		condition(cmd, envp, id_builtin, fullpath);
		exit(g_exit_status);
	}
	else
		parent_process(exec, cmd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/*void	execute_fork(t_cmd *cmd, t_exec *exec, char **envp, int i)
{
	char	*fullpath;
	int		id_builtin;

	id_builtin = is_builtin(cmd->args[0]);
	if (exec->pid[i] == 0)
	{
		g_exit_status = 0;
		if (cmd->heredoc)
		{
			dup2(cmd->hdoc_pipe[0], STDIN_FILENO);
			close(cmd->hdoc_pipe[0]);
		}
		setup_redirections(cmd, exec);
		if (control_infiles(cmd))
			exit(1);
		else if (!cmd->args || !cmd->args[0])
			exit(0);
		if (id_builtin == 0)
		{
			fullpath = get_cmd_path(cmd->args[0], envp);
			controlpath(fullpath, cmd);
			if (g_exit_status == 127)
				exit(127);
		}
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (id_builtin >= 1 && id_builtin <= 3)
			exit(exec_builtin(cmd, &envp));
		else
			execute_execve(fullpath, cmd, envp);
		exit(g_exit_status);
	}
	else
		parent_process(exec, cmd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}*/
