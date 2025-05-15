/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 21:39:46 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	char	*fullpath;

	pid = fork();
	if (pid == 0)
	{
		fullpath = get_cmd_path(cmd->args[0], envp);
		if (!fullpath)
		{
			perror("command");
			exit(1);
		}
	execve(fullpath, cmd->args, envp);
	perror("execve");
	exit (1);
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}
