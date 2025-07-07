/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <antuel@outlook.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:10:01 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/07 20:06:33 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Vérifie que l’entrée est non vide et sans quotes non fermées
static int	sanitize_input(char *line)
{
	if (!line || !line[0])
		return (0);
	if (contains_forbidden_chars(line))
		return (0);
	if (check_unclosed_quotes(line))
	{
		printf("Syntax error: unclosed quote\n");
		return (0);
	}
	return (1);
}

// Fonction principale de parsing : transforme une ligne en liste de commandes
t_cmd	*parse_line(char *line, char **envp, int exit_code)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (!sanitize_input(line))
		return (NULL);
	tokens = tokenize(line);
	if (!tokens)
		return (free(line), NULL);
	if (!syntax_is_valid(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	tokens = expand_tokens(tokens, envp, exit_code);
	if (!tokens)
		return (NULL);
	remove_quotes_from_tokens(tokens);
	cmds = build_cmd_list_from_tokens(tokens);
	free_tokens(tokens);
	if (!cmds)
		printf("Parsing failed.\n");
	return (cmds);
}
