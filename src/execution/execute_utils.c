/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:28:24 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/28 17:28:24 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	init_exec(t_exec *exec, int count)
{
	int	i;

	i = 0;
	exec->cmd_count = count;
	exec->pid = malloc(sizeof(pid_t) * count);
	if (!exec->pid)
		return (perror("malloc"), 1);
	while (i < count)
		exec->pid[i++] = -1;
	exec->fd_in = STDIN_FILENO;
	exec->pipe_fd[0] = -1;
	exec->pipe_fd[1] = -1;
	return (0);
}

int	control_builtin(t_cmd *cmd_list, char ***envp)
{
	int	id;
	int	exit_code;

	id = is_builtin(cmd_list->args[0]);
	if (!cmd_list->next && !cmd_list->infile && !cmd_list->outfile && id > 0)
	{
		if (id == 5)
		{
			if (ft_exit(cmd_list->args, envp, &exit_code) == 0)
				quit_minishell(*envp, exit_code);
			return (1);
		}
		else
			g_exit_status = exec_builtin(cmd_list, envp);
		return (1);
	}
	return (0);
}


/*control = control d erreurs pour le exit_status*/
int	control_infiles(t_cmd *cmd)
{
	int	control;

	control = 0;
	if (cmd->outfile != NULL && cmd->append != -1)
	{
		if (handle_outfile(cmd))
			control = 1;
	}
	if (cmd->infile)
	{
		if (handle_infile(cmd))
			control = 1;
	}
	if (control)
		return (g_exit_status = 1, 1);
	return (0);
}
