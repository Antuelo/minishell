/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:13:51 by anoviedo          #+#    #+#             */
/*   Updated: 2025/06/07 12:22:49 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

int		g_exit_status = 0;

// Affiche le contenu de la structure t_cmd pour debug
void	print_cmd(t_cmd *cmd)
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
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd;
	char	*input;
	char	**my_envp;

	(void)argc;
	(void)argv;
	my_envp = copy_envp(envp);
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
