/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:49:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/20 16:50:00 by anoviedo         ###   ########.fr       */
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

int	control_signs(int status, t_exec *exec, int j)
{
	int	ss;

	ss = 0;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
	{
		if (WCOREDUMP(status))
			write(1, "Quit (core dumped)\n", 19);
		else
			write (1, "Quit\n", 5);
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
/*void	wait_all_processes(t_exec *exec)
{
	int	j;
	int	status;
	int	saw_sigint;

	j = 0;
	saw_sigint = 0;
	status = 0;
	while (j < exec->cmd_count)
	{
		waitpid(exec->pid[j], &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			saw_sigint = 1;
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
}*/
