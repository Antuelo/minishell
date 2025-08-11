/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:10:49 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/11 12:08:07 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

void	clean_exit(t_cmd *cmd, char **envp, int code)
{
	free_cmd_full(cmd);
	if (envp)
		free_envp(envp, count_env(envp));
	exit(code);
}

/* je fais 	g_exit_status = 0;
** pour éviter que un erreur d auparavant reste "collé" aux
** futurs procesus */
static void	stat_result(char *path, struct stat *sb, t_cmd *cmd, char **envp)
{
	if (S_ISDIR(sb->st_mode))
	{
		g_exit_status = 126;
		fprintf(stderr, "%s: Is a directory\n", path);
		clean_exit_child(cmd, envp, 126);
	}
	else if (access(path, X_OK) != 0)
	{
		g_exit_status = 126;
		fprintf(stderr, "%s: Permission denied\n", path);
		g_exit_status = 1;
		clean_exit_child(cmd, envp, 126);
	}
}

void	controlpath(char *path, t_cmd *cmd, char **envp)
{
	struct stat	sb;

	g_exit_status = 0;
	if (!path)
	{
		g_exit_status = 127;
		fprintf(stderr, "%s: command not found\n", cmd->args[0]);
		clean_exit_child(cmd, envp, 127);
	}
	if (stat(path, &sb) == 0)
		stat_result(path, &sb, cmd, envp);
	else
	{
		g_exit_status = 127;
		fprintf(stderr, "%s: No such file or directory\n", path);
		clean_exit_child(cmd, envp, 127);
	}
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

/*	ENOENT : fichier inexistant

	ENOTDIR : Une partie du chemin n'est pas un dossier alors qu'elle
	devrait l'être
	_exit ----> pour eviter des handlers de readline
*/
void	execute_execve(char *fullpath, t_cmd *cmd, char **envp)
{
	execve(fullpath, cmd->args, envp);
	perror("execve");
	free(fullpath);
	free_cmd_full(cmd);
	free_envp(envp, count_env(envp));
	if (errno == ENOENT || errno == ENOTDIR)
		_exit(127);
	else
		_exit(1);
}
