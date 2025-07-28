/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_childandparent.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:28:37 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/28 17:28:37 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	condition(t_cmd *cmd, char **envp, int id_builtin, char *fullpath)
{
	int	count;
	int	status;
	int	exit_code;

	if (id_builtin > 0)
	{
		if (id_builtin == 5)
		{
			if (ft_exit(cmd->args, &envp, &exit_code) == 0)
				quit_minishell(envp, exit_code);
		}
		count = count_env(envp);
		status = exec_builtin(cmd, &envp);
		free_envp(envp, count);
		free(fullpath);
		exit(status);
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
		if (cmd->args[0][0] == '/' || ft_strncmp(cmd->args[0], "./", 2) == 0)
			fullpath = ft_strdup(cmd->args[0]);
		else
			fullpath = get_cmd_path(cmd->args[0], envp);
		controlpath(fullpath, cmd, envp);
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
		if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
			exit(0);
		fullpath = control_path(cmd, envp, id_builtin);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		condition(cmd, envp, id_builtin, fullpath);
		free(fullpath);
		exit(g_exit_status);
	}
	parent_process(exec, cmd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
