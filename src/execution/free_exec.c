/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:49:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/20 17:21:00 by anoviedo         ###   ########.fr       */
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
| Nom du signal | Numéro | Signification                               | Effet typique                     |
| ------------- | ------ | ------------------------------------------- | --------------------------------- |
| `SIGHUP`      | 1      | Hangup (déconnexion du terminal)            | Termine le processus              |
| `SIGINT`      | 2      | Interruption (Ctrl + C)                     | Termine le processus (130)        |
| `SIGQUIT`     | 3      | Quitter (Ctrl + \\)                         | Termine + core dump (131)         |
| `SIGILL`      | 4      | Instruction illégale                        | Termine + core dump               |
| `SIGABRT`     | 6      | Abandon (abort)                             | Termine + core dump               |
| `SIGFPE`      | 8      | Erreur arithmétique (ex: division par zéro) | Termine + core dump               |
| `SIGKILL`     | 9      | Forcer la fin immédiate                     | Termine immédiatement             |
| `SIGSEGV`     | 11     | Segmentation fault                          | Termine + core dump (139)         |
| `SIGPIPE`     | 13     | Écriture sur un pipe sans lecteur           | Termine silencieusement           |
| `SIGALRM`     | 14     | Timeout (alarme)                            | Termine le processus              |
| `SIGTERM`     | 15     | Demande de terminaison classique            | Termine le processus (par défaut) |
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
	if (j == exec->cmd_count -1)
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
void	wait_all_processes(t_exec *exec)
{
	int	status;
	int	saw_sigint;
	int	j;

	status = 0;
	j = 0;
	while (j < exec->cmd_count)
	{
		waitpid(exec->pid[j], &status, 0);
		saw_sigint = control_signs(status, exec, j);
		j++;
	}
	if (saw_sigint)
		write(1, "\n", 1);
}

