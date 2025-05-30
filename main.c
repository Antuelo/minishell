/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:13:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/30 15:03:15 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status;
char	**g_envp = NULL;

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;
	char	*input;

	(void)argc;
	(void)argv;
	g_envp = copy_envp(envp);
	g_exit_status = 0;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (input[0] != '\0')
			add_history(input);
		else
		{
			free(input);
			continue ;
		}
		// Simulación del parser — reemplazá esto por tu parser real después
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
			return (free(input), 1);
		cmd->args = ft_split(input, ' ');
		cmd->infile = NULL;
		cmd->outfile = NULL;
		cmd->append = -1;
		cmd->heredoc = 0;
		cmd->delimiter = NULL;
		cmd->next = NULL;
		cmd->prev = NULL;
//		printf("comando recibido: [%s]\n", cmd->args[0]);
		if (cmd->args && ft_strncmp(cmd->args[0], "exit", 5) == 0)
		{
			free_cmd(cmd);
			free(input);
			break ;
		}
		execute(cmd);
		free_cmd(cmd);
		free(input);
	}
	clear_history();
	rl_clear_history();
	return (0);
}
