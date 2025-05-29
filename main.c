/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:13:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/29 15:10:19 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status;
char	**g_envp = NULL;

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;
	char	*input;

	g_envp = copy_envp(envp);
	(void)argc;
	(void)argv;
	g_exit_status = 0;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input || input[0] == '\0')
		{
			free(input);
			continue ;
		}
		if (input)
			add_history(input);
		// Simulación de parser:
		cmd = malloc(sizeof(t_cmd));
		cmd->args = ft_split("env", ' ');
		cmd->infile = NULL;
		cmd->outfile = NULL;
		cmd->append = -1;
		cmd->heredoc = 0;
		cmd->delimiter = NULL;
		cmd->next = NULL;
		cmd->prev = NULL;
		execute(cmd);
		free_cmd(cmd);
		if (cmd->args)
		{
			clear_history();
			rl_clear_history();
			free(input);
			free_cmd(cmd);
			break ;
		}
		if (execute(cmd) == 1)
		{
			clear_history();
			rl_clear_history();
			free(input);
			free_cmd(cmd);
			return (1);
		}
		if (cmd->args && ft_strncmp(cmd->args[0], "exit", 5) == 0)
			exit(0);
		free_cmd(cmd);
		free(input);
	}
	return (0);
}

/*
int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;
	char	*input;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input || (cmd && cmd->args && ft_strncmp(cmd->args[0], "exit",
					5) == 0)) // si l'utilisateur tape Ctrl+D ou écrit "exit",
						on quitte la boucle
			break ;
		if (input)
			add_history(input);
		cmd = malloc(sizeof(t_cmd)); // SIMULATION DU PARSER ...
		cmd->args = ft_split("echo hola", ' ');
		cmd->infile = NULL;
		cmd->outfile = "salida.txt";
		cmd->append = 0;
		cmd->heredoc = 0;
		cmd->delimiter = NULL;
		cmd->next = NULL;
		cmd->prev = NULL;
		if (execute(cmd, envp) == 1)
			return (free_cmd(cmd), 1);
		free_cmd(cmd);
	}
	return (0);
}
*/
