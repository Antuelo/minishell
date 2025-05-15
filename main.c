/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:13:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/05/15 23:55:54 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;
	char	*input;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input) // JE LAISSE POUR LE CTRL-D
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
		execute(cmd, envp);
		free_cmd(cmd);
	}
	return (0);
}
