/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:02:35 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/20 19:02:35 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

#define COLOR_GREEN "\033[1;32m"
#define COLOR_RESET "\033[0m"

int	g_exit_status;

/* rl_catch_signals = 0; c'est pour pouvoir controler moi même
** le hendler, c'est à dire, les signals... EN REALITé DESACTIVE
** LES HANDLERS INTERNES

** 	signal(SIGINT, handle_signs) = installe mon prope handler pour SIGINT
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
	while (1)
	{
		signal(SIGINT, handle_signs);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell$ ");
		if (!input)
		{
			free_envp(my_envp, count_env(my_envp));
			write(1, "exit\n", 5);
			return (0);
		}
		if (input[0] != '\0')
		{
			add_history(input);
			cmds = parse_line(input, my_envp, g_exit_status);
			if (cmds)
			{
				execute_pipeline(cmds, &my_envp);
				free_cmd_list(cmds);
			}
			else
				g_exit_status = 2;
		}
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
