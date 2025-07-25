/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:16:06 by anoviedo          #+#    #+#             */
/*   Updated: 2025/07/25 19:55:23 by anoviedo         ###   ########.fr       */
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
		free_envp(envp, count_env(envp));
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
