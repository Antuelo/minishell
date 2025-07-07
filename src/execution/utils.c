/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:00:53 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/07 18:00:57 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

void	clean_exit(t_cmd *cmd, char **envp, int code)
{
	free_cmd_list(cmd);
	if (envp)
		free_envp(envp, count_env(envp));
	exit(code);
}

/* je fais 	g_exit_status = 0;
** pour éviter que un erreur d auparavant reste "collé" aux
** futurs procesus */
void	controlpath(char *path, t_cmd *cmd, char **envp)
{
	g_exit_status = 0;
	if (!path)
	{
		g_exit_status = 127;
		perror("error: command not found");
		clean_exit(cmd, envp, 127);
	}
}

char	**extract_paths(char **envp)
{
	int		i;
	char	**path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	return (path);
}

int	countcmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	execute_execve(char *fullpath, t_cmd *cmd, char **envp)
{
	execve(fullpath, cmd->args, envp);
	perror("execve");
	free(fullpath);
	free_cmd(cmd);
	free_envp(envp, count_env(envp));
	if (errno == ENOENT || errno == ENOTDIR)
		exit(127);
	else
		exit(1);
}
