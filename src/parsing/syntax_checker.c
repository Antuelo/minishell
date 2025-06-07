/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoviedo <anoviedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:25:05 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/07 11:23:30 by anoviedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include "parsing.h"

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

// Vérifie qu’aucune redirection ne suit directement un pipe
int	check_pipe_redirection(t_token *tokens)
{
	t_token	*curr;
	t_token	*next;

	curr = tokens;
	while (curr)
	{
		if (curr->type == T_PIPE && curr->next)
		{
			next = curr->next;
			while (next && next->type == T_WORD
				&& (!next->value || !*next->value))
				next = next->next;
			if (next && (next->type == T_REDIR_IN || next->type == T_REDIR_OUT
					|| next->type == T_REDIR_APPEND || next->type == T_HEREDOC))
			{
				printf("Syntax error: unexpected redirection after pipe\n");
				return (0);
			}
		}
		curr = curr->next;
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
				return (0);
			}
		}
		curr = curr->next;
	}
	return (1);
}
