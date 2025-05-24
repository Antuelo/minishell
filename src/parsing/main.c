/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:14:43 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/24 16:15:11 by llabatut         ###   ########.ch       */
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*tokens;
	t_cmd	*cmd;
	int		last_exit_code;

	(void)argc;
	(void)argv;
	last_exit_code = 0; // Sert à remplacer $?

	while (1)
	{
		// Affiche le prompt et lit la ligne utilisateur
		line = get_user_input();
		if (!line[0])
		{
			free(line);
			continue ;
		}
		if (check_unclosed_quotes(line))
		{
			printf("Syntax error: unclosed quote\n");
			free(line);
			continue ;
		}

		// Découpe la ligne en tokens
		tokens = tokenize(line);
		if (!tokens)
		{
			free(line);
			continue ;
		}

		// Remplace les variables d'environnement dans les tokens ($VAR, $?)
		expand_tokens(tokens, envp, last_exit_code);
		remove_quotes_from_tokens(tokens);
		// Alloue une structure de commande à remplir depuis les tokens
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
			return (free_tokens(tokens), free(line), 1);

		// Construit la structure de commande à partir des tokens
		if (!fill_cmd_from_tokens(tokens, cmd))
			printf("Parsing failed.\n");
		else
			print_cmd(cmd);

		// Nettoyage mémoire
		free_cmd(cmd);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
