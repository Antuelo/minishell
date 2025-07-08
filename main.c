/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:01:28 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 16:01:28 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

#define CLR_GREEN "\001\033[1;32m\002"
#define CLR_RESET "\001\033[0m\002"

int			g_exit_status;

static void	second_control(t_cmd *cmds, char ***my_envp)
{
	if (cmds)
	{
		execute_pipeline(cmds, my_envp);
		free_cmd_list(cmds);
	}
	else
		g_exit_status = 1;
}

static char	*first_control(char **my_envp)
{
	char	*prompt;
	char	*input;

	prompt = CLR_GREEN "minishell$ " CLR_RESET;
	input = readline(prompt);
	if (!input)
	{
		free_envp(my_envp, count_env(my_envp));
		write(1, "exit\n", 5);
		return (NULL);
	}
	return (input);
}

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
	signal(SIGINT, handle_signs);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = first_control(my_envp);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			cmds = parse_line(input, my_envp, g_exit_status);
			second_control(cmds, &my_envp);
		}
		free(input);
	}
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