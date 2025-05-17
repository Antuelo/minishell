/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:39:17 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/17 18:09:08 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_cmd *cmd, char **envp)
{
	execute_pipeline(cmd, envp);
	return (0);
}


/*int	execute(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	char	*fullpath;

	while (cmd)
	{
		if (pipe(cmd->hdoc_pipe) == -1)
			return (perror("pipe"), 1);
		if ((pid = fork()) < 0)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			fullpath = get_cmd_path(cmd->args[0], envp); // obtenir le path
			controlpath(fullpath);                       // control d'erreur
			if (cmd->outfile != NULL && cmd->append != -1)
				handle_outfile(cmd); // si parsing ">" or ">>"
			if (cmd->infile)
				handle_infile(cmd);
			execve(fullpath, cmd->args, envp);
			perror("execve");
			freepath(&fullpath);
			free_cmd(cmd);
			exit(1);
		}
		else
			waitpid(pid, NULL, 0);
	}
	return (0);
}*/
