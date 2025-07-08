/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:33:18 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 20:33:18 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Vérifie si un token est un pipe
static int	is_pipe(t_token *token)
{
	return (token && token->type == T_PIPE);
}

// Vérifie les erreurs de syntaxe liées aux pipes
int	check_pipe_errors(t_token *tokens)
{
	t_token	*curr;

	if (!tokens)
		return (0);
	curr = tokens;
	if (is_pipe(curr) || (!curr->next && is_pipe(curr)))
	{
		printf("Syntax error: unexpected token `|'\n");
		return (0);
	}
	while (curr && curr->next)
	{
		if (is_pipe(curr) && is_pipe(curr->next))
		{
			printf("Syntax error: unexpected token `|'\n");
			return (0);
		}
		curr = curr->next;
	}
	if (curr && is_pipe(curr))
	{
		printf("Syntax error: unexpected token `|'\n");
		return (0);
	}
	return (1);
}

// Vérifie la validité syntaxique globale de la ligne de tokens
int	syntax_is_valid(t_token *tokens)
{
	t_token	*curr;

	if (!tokens)
		return (0);
	if (!check_pipe_errors(tokens))
		return (0);
	if (!check_pipe_redirection(tokens))
		return (0);
	curr = tokens;
	while (curr)
	{
		if (curr->type >= T_REDIR_IN && curr->type <= T_HEREDOC)
		{
			if (!curr->next || curr->next->type != T_WORD)
			{
				printf("Syntax error: expected filename after redirection\n");
				return (2);
			}
		}
		curr = curr->next;
	}
	return (1);
}

int	contains_forbidden_chars(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (line[i])
	{
		if (line[i] == ';' || line[i] == '\\'
			|| line[i] == '(' || line[i] == ')')
		{
			printf("Syntax error near unexpected token `%c'\n", line[i]);
			return (1);
		}
		if ((line[i] == '&' && line[i + 1] == '&')
			|| (line[i] == '|' && line[i + 1] == '|'))
		{
			printf("Syntax error near unexpected token `%.2s'\n", &line[i]);
			return (1);
		}
		i++;
	}
	return (0);
}
