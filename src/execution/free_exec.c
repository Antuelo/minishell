/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:43:54 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/10 22:53:16 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

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

/* * update_status

 Analyse le `status` renvoyé par waitpid() pour un enfant donné.

  Paramètres :
	- `status`      : valeur brute renvoyée par waitpid().
	- `idx`         : indice de cet enfant dans exec->pid[].
	- `last_exit`   : pointeur vers le dernier exit() reçu (sortie normale).
	- `last_idx`    : pointeur vers l’indice du dernier exit() reçu.
	- `saw_sigint`  : pointeur booléen qui devient 1 si un enfant a reçu SIGINT.

  Actions :
	1. Si l’enfant s’est terminé par un signal :
			• SIGINT  → on note l’interruption pour afficher un ‘\n’ plus tard.
			• SIGQUIT → on écrit “Quit (core dumped)”.
	2. Si l’enfant s’est terminé normalement (exit) :
			• On mémorise son code de sortie et son indice
			(c’est le « dernier exit valide »).*/
static int	update_status(int status, int idx, int *last_exit, int *last_idx)
{
	int	saw_sigint;
	int	sig;

	saw_sigint = 0;
	sig = WTERMSIG(status);
	if (WIFSIGNALED(status))
	{
		if (sig == SIGINT)
			saw_sigint = 2;
		else if (sig == SIGQUIT && WCOREDUMP(status))
		{
			write(1, "Quit (core dumped)", 19);
			saw_sigint = 1;
			return (*last_idx = 424242, *last_exit = 131, saw_sigint);
		}
		*last_exit = 128 + sig;
		*last_idx = idx;
	}
	else if (WIFEXITED(status))
	{
		*last_exit = WEXITSTATUS(status);
		*last_idx = idx;
	}
	return (saw_sigint);
}

/** Établit la valeur finale de `g_exit_status` une fois que tous les enfants
 * sont récoltés, et restaure l’affichage.
 *
 * Paramètres :
 *   - `saw_sigint` : 1 si au moins un enfant a fini sur SIGINT.
 *   - `last_exit`  : code du dernier exit() normal reçu.
 *   - `last_idx`   : indice de cet enfant.
 *   - `cmd_count`  : nombre total de commandes dans le pipeline.
 *
 * Règles (mimique Bash) :
 *   • Si SIGINT : on imprime une ligne vide pour replacer le prompt.
 *   • Si le dernier exit() n’appartient PAS à la dernière commande du pipe,
 *     cela signifie qu’une commande précédente a échoué → g_exit_status = 1.
 *   • Sinon, g_exit_status reçoit `last_exit`.*/
static void	set_global_exit(int saw_sigint, int last_exit, int last_idx,
		int cmd_count)
{
	if (saw_sigint)
		write(1, "\n", 1);
	if (last_idx == cmd_count - 1 || last_idx == 424242)
		g_exit_status = last_exit;
	else if (saw_sigint == 2)
		g_exit_status = 130;
	else
		g_exit_status = 1;
	signal(SIGINT, handle_signs);
	signal(SIGQUIT, SIG_IGN);
}

/** Boucle principale qui attend chaque processus fils d’un pipeline, met à jour
 * les informations d’état via update_status(), puis appelle set_global_exit().
 *
 * Étapes :
 *   1. Initialisation des accumulateurs (SIGINT, last_exit, last_idx…).
 *   2. Pour chaque PID non nul :
 *        • waitpid(pid) bloque jusqu’à la fin de ce fils.
 *        • update_status() traite le status et met à jour les accumulateurs.
 *   3. Une fois tous les fils terminés :
 *        • set_global_exit() décide du g_exit_status final
 *          et imprime/masque ce qu’il faut.
 *   4. Ne traite qu’un maximum de 25 lignes → conforme Norme 42.*/
void	wait_all_processes(t_exec *exec)
{
	int	status;
	int	saw_sigint;
	int	last_exit;
	int	last_idx;
	int	i;

	i = -1;
	saw_sigint = 0;
	last_exit = 0;
	last_idx = -1;
	while (++i < exec->cmd_count)
	{
		if (exec->pid[i] != -1)
		{
			waitpid(exec->pid[i], &status, 0);
			saw_sigint = update_status(status, i, &last_exit, &last_idx);
			if (saw_sigint)
				break ;
		}
	}
	set_global_exit(saw_sigint, last_exit, last_idx, exec->cmd_count);
	free(exec->pid);
}
