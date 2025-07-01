/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/28 11:10:24 by anoviedo         ###   ########.fr       */
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

	id = is_builtin(cmd_list->args[0]);
	if (!cmd_list->next && !cmd_list->infile && !cmd_list->outfile && id > 0)
	{
		if (id == 5)
			ft_exit(cmd_list->args);
		else
			g_exit_status = exec_builtin(cmd_list, envp);
		return (1);
	}
	return (0);
}

int	control_infiles(t_cmd *cmd)
{
	if (cmd->outfile != NULL && cmd->append != -1)
		handle_outfile(cmd);
	if (cmd->infile)
		handle_infile(cmd);
	if (g_exit_status == 1)
		return (1);
	return (0);
}

