/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:22:47 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/28 18:22:54 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Libère la structure t_cmd
void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->delimiter)
		free(cmd->delimiter);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}

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

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd *tmp;
	while (cmd)
	{
		tmp = cmd->next;
		free_cmd(cmd);
		cmd = tmp;
	}
}


int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmds;
	int		last_exit_code;

	(void)argc;
	(void)argv;
	last_exit_code = 0;

	while (1)
	{
		// Affiche le prompt et lit la ligne utilisateur
		line = get_user_input();
		if (!line[0])
		{
			free(line);
			continue ;
		}

		// Vérifie les quotes non fermées avant de tokenizer
		if (check_unclosed_quotes(line))
		{
			printf("Syntax error: unclosed quote\n");
			free(line);
			continue ;
		}

		// Tokenize la ligne
		tokens = tokenize(line);
		if (!tokens)
		{
			free(line);
			continue ;
		}

		// Vérification centralisée de la syntaxe
		if (!syntax_is_valid(tokens))
		{
			free_tokens(tokens);
			free(line);
			continue ;
		}

		// Expansion des variables
		expand_tokens(tokens, envp, last_exit_code);
		remove_quotes_from_tokens(tokens);

		// Parsing en liste de commandes
		cmds = build_cmd_list_from_tokens(tokens);
		if (!cmds)
			printf("Parsing failed.\n");
		else
		{
			t_cmd *tmp = cmds;
			while (tmp)
			{
				print_cmd(tmp);
				printf("--------\n");
				tmp = tmp->next;
			}
			free_cmd_list(cmds);
		}
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
