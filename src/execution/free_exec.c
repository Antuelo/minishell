/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:43:54 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 20:45:14 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	freepath(char **patch)
{
	int	i;

	i = 0;
	while (patch[i])
		free(patch[i++]);
	free(patch);
}

void	free_envp(char **envp, int count)
{
	while (--count >= 0)
		free(envp[count]);
	free(envp);
}

/*
| Nom du signal | Numéro | Signification                               |
| ------------- | ------ | ------------------------------------------- |
| `SIGHUP`      | 1      | Hangup (déconnexion du terminal)            |
| `SIGINT`      | 2      | Interruption (Ctrl + C)                     |
| `SIGQUIT`     | 3      | Quitter (Ctrl + \\)                         |
| `SIGILL`      | 4      | Instruction illégale                        |
| `SIGABRT`     | 6      | Abandon (abort)                             |
| `SIGFPE`      | 8      | Erreur arithmétique (ex: division par zéro) |
| `SIGKILL`     | 9      | Forcer la fin immédiate                     |
| `SIGSEGV`     | 11     | Segmentation fault                          |
| `SIGPIPE`     | 13     | Écriture sur un pipe sans lecteur           |
| `SIGALRM`     | 14     | Timeout (alarme)                            |
| `SIGTERM`     | 15     | Demande de terminaison classique            |
*/
int	control_signs(int status, t_exec *exec, int j)
{
	int	ss;

	ss = 0;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
	{
		if (WCOREDUMP(status))
		{
			write(1, "Quit (core dumped)\n", 19);
			g_exit_status = 131;
		}
		return (0);
	}
	if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT))
		ss = 1;
	if (j == exec->cmd_count - 1)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
	return (ss);
}

/**
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	c'est pour faire un saute de ligne si ctrl + c, s est fait dasn le fils
	et il doit pas se répeter dans le père
*/
static void	update_last_exit(int *status, int *exit_code, int *last_indx, int i)
{
	if (WIFEXITED(*status))
	{
		*exit_code = WEXITSTATUS(*status);
		*last_indx = i;
	}
}

void	wait_all_processes(t_exec *exec, t_cmd *cmd_list)
{
	int		i;
	int		status;
	t_cmd	*cmd;
	int		last_valid_exit;
	int		last_valid_index;

	i = 0;
	cmd = cmd_list;
	last_valid_exit = 0;
	last_valid_index = -1;
	while (i < exec->cmd_count)
	{
		if (exec->pid[i] != -1)
		{
			waitpid(exec->pid[i], &status, 0);
			update_last_exit(&status, &last_valid_exit, &last_valid_index, i);
		}
		if (cmd)
			cmd = cmd->next;
		i++;
	}
	if (last_valid_index != exec->cmd_count - 1)
		g_exit_status = 1;
	else
		g_exit_status = last_valid_exit;
}
