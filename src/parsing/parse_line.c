/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:23:44 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 21:23:57 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Vérifie que l’entrée est non vide et sans quotes non fermées
static int	sanitize_input(char *line)
{
	if (!line || !line[0])
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
		return (NULL);
	if (!syntax_is_valid(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	expand_tokens(tokens, envp, exit_code);
	remove_quotes_from_tokens(tokens);
	cmds = build_cmd_list_from_tokens(tokens);
	free_tokens(tokens);
	if (!cmds)
		printf("Parsing failed.\n");
	return (cmds);
}
