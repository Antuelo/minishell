/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/13 23:43:18 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int	execute(t_cmd *cmd, char ***envp)
{
	g_exit_status = execute_pipeline(cmd, envp);
	return (g_exit_status);
}

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

t_cmd	**list_to_array(t_cmd *cmd, int count)
{
	t_cmd	**array;
	int		i;

	array = malloc(sizeof(t_cmd *) * count);
	if (!array)
		return (NULL);
	i = 0;
	while (cmd)
	{
		array[i] = cmd;
		cmd = cmd->next;
		i++;
	}
	return (array);
}
