/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:46:10 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/25 00:16:36 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/* je fais 	g_exit_status = 0;
** pour éviter que un erreur d auparavant reste "collé" aux
** futurs procesus */
void	controlpath(char *path, t_cmd *cmd)
{
	g_exit_status = 0;
	if (!path)
	{
		g_exit_status = 127;
		free_cmd(cmd);
		perror("error: command not found");
		exit(127);
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

/*condition peut etre à ajouter

	*/
