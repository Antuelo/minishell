/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:49:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/07 10:58:10 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	if (cmd->args)
	{
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	free(cmd);
}

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

/*Estoy esperando a que los procesos hijos terminen
con waitpid, y luego uso macros de <sys/wait.h> para
 analizar el resultado. Si el proceso terminó
 normalmente, guardo su exit code.
 Si fue interrumpido por una señal (como Ctrl+C),
 entonces devuelvo 128 + signal, como hace Bash.
 Esto me permite simular correctamente el comportamiento
 de salida de comandos en una shell real.

** 📌 Rappel pour les macros de <sys/wait.h> :
**
** WIFEXITED(status)
** → "Wait If Exited" : Retourne true si le processus s'est terminé
normalement avec exit().
**
** WEXITSTATUS(status)
** → "Wait Exit Status" : Donne le code de retour passé à exit() (ex :
exit(1) → 1).
**   ⚠️ À utiliser seulement si WIFEXITED(status) est vrai.
**
** WIFSIGNALED(status)
** → "Wait If Signaled" : Retourne true
si le processus a été terminé par un signal (ex: Ctrl+C).
**
** WTERMSIG(status)
** → "Wait Termination Signal" : Donne le numéro du
signal ayant tué le processus.
**   (ex : SIGINT = 2 → Bash retourne 130, car 128 + 2).
**
** 💡 Ces macros permettent de simuler le comportement de Bash dans une minishell.
*/
void	wait_all_processes(t_exec *exec)
{
	int	j;
	int	status;

	j = 0;
	while (j < exec->cmd_count)
	{
		waitpid(exec->pid[j], &status, 0);
		if (j == exec->cmd_count -1)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
		}
		j++;
	}
}

/*
version de free_envp sans count, mais je l'utilse pas
pour le moment... on verra...
void	free_envp_nullterm(char **envp)
{
	int	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}*/
