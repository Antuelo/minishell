/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/05 14:06:35 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_exec(t_exec *exec, int count)
{
	exec->cmd_count = count;
	exec->pid = malloc(sizeof(pid_t) * count);
	if (!exec->pid)
		return (perror("malloc"), 1);
	exec->fd_in = 0;
	return (0);
}

int	control_builtin(t_cmd *cmd_list, char ***envp)
{
	int	id;

	id = is_builtin(cmd_list->args[0]);
	if (countcmds(cmd_list) == 1 && (id >= 4 && id <= 7))
	{
		if (id == 5)
			ft_exit(cmd_list->args);
		else
			exec_builtin(cmd_list, envp);
		return (1);
	}
	return (0);
}

int	execute(t_cmd *cmd, char ***envp)
{
	g_exit_status = execute_pipeline(cmd, envp);
	return (g_exit_status);
}
