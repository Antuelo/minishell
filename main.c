/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:13:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/05 13:47:07 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;
	char	*input;
	char	**my_envp;

	(void)argc;
	(void)argv;
	my_envp = copy_envp(envp);
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
		// Simulación del parser — à remplacer plus tard
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
		execute(cmd, &my_envp);
		free_cmd(cmd);
		free(input);
	}
	clear_history();
	rl_clear_history();
	return (0);
}
