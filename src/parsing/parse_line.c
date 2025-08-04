/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:04:27 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/28 18:04:27 by llabatut         ###   ########.ch       */
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

static int	prep_tokens(char *line, char **envp, int exit_code, t_token **out)
{
	if (!sanitize_input(line))
		return (0);
	*out = tokenize(line);
	if (!*out)
		return (0);
	if (!syntax_is_valid(*out))
		return (free_tokens(*out), 0);
	*out = expand_tokens(*out, envp, exit_code);
	if (!*out)
		return (0);
	return (1);
}

// Fonction principale de parsing : transforme une ligne en liste de commandes
t_cmd	*parse_line(char *line, char **envp, int exit_code)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (!prep_tokens(line, envp, exit_code, &tokens))
		return (NULL);
	cmds = build_cmd_list_from_tokens(tokens);
	free_tokens(tokens);
	if (!cmds)
		return (g_exit_status = 1, NULL);
	return (cmds);
}
