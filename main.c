/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:13:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/17 13:53:26 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

#define COLOR_GREEN   "\033[1;32m"
#define COLOR_RESET   "\033[0m"

int		g_exit_status;

/*	rl_replace_line("", 0) nettoie ce qui a fait l utilisateur
** 	rl_on_new_line(); c'est pour informer que on es dans une nouvelle ligne
	rl_redisplay(); pour reimprimer le prompt
*/
void	handle_signs(int sign)
{
	(void)sign;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* rl_catch_signals = 0; c'est pour pouvoir controler moi même
** le hendler, c'est à dire, les signals...
*/
int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmds;
	char	*input;
	char	**my_envp;

	g_exit_status = 0;
	(void)argc;
	(void)argv;
	my_envp = copy_envp(envp);
	rl_catch_signals = 0;
	signal(SIGINT, handle_signs);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		cmds = parse_line(input, my_envp, g_exit_status);
		if (!cmds)
		{
			free(input);
			continue ;
		}
		execute(cmds, &my_envp);
		free_cmd_list(cmds);
		free(input);
	}
	clear_history();
	rl_clear_history();
	return (0);
}

// Affiche le contenu de la structure t_cmd pour debug
/*void	print_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	printf("Infile    : %s\n", cmd->infile);
	printf("Outfile   : %s\n", cmd->outfile);
	printf("Append    : %d\n", cmd->append);
	printf("Heredoc   : %d\n", cmd->heredoc);
	printf("Delimiter : %s\n", cmd->delimiter);
	printf("Args      : ");
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			printf("[%s] ", cmd->args[i++]);
		printf("\n");
	}
}*/

