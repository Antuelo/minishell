/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:59:50 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/10 21:52:32 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/*	rl_replace_line("", 0) nettoie ce qui a fait l utilisateur
** 	rl_on_new_line(); c'est pour informer que on es dans une nouvelle ligne
	rl_redisplay(); pour reimprimer le prompt
*/
void	handle_signs(int signo)
{
	(void)signo;
	write(STDERR_FILENO, "^C\n", 3);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130;
}

void	quit_minishell(char **envp, int code)
{
	if (envp)
	{
		free_envp(envp, count_env(envp));
		envp = NULL;
	}
	clear_history();
	rl_clear_history();
	exit(code);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("pwd"), 1);
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}

void	clean_exit_child(t_cmd *cmd, char **envp, int code)
{
	free_cmd_full(cmd);
	if (envp)
		free_envp(envp, count_env(envp));
	_exit(code);
}

void	close_all_heredoc_fds(t_cmd *list)
{
	t_cmd	*c;

	c = list;
	while (c)
	{
		if (c->hdoc_pipe[0] != -1)
		{
			close(c->hdoc_pipe[0]);
			c->hdoc_pipe[0] = -1;
		}
		if (c->hdoc_pipe[1] != -1)
		{
			close(c->hdoc_pipe[1]);
			c->hdoc_pipe[1] = -1;
		}
		if (c->delimiter)
		{
			freepath(c->delimiter);
			c->delimiter = NULL;
		}
		c = c->next;
	}
}
