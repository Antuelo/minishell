/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:49:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/18 20:34:49 by anoviedo         ###   ########.fr       */
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
**   À utiliser seulement si WIFEXITED(status) est vrai.
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
** Ces macros permettent de simuler le comportement de Bash dans une minishell.

	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	c'est pour faire un saute de ligne si ctrl + c, s est fait dasn le fils
	et il doit pas se répeter dans le père
*/
void	wait_all_processes(t_exec *exec)
{
	int	j;
	int	status;
	int	saw_sigint;

	j = 0;
	saw_sigint = 0;
	status = 0;
	while (j < exec->cmd_count)
	{
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			saw_sigint = 1;
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
	if (saw_sigint)
		write(1, "\n", 1);
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
