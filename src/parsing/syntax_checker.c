/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 27/05/2025 21:59:14 by llabatut          #+#    #+#             */
/*   Updated: 28/05/2025 18:41:26 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"
static int	is_pipe(t_token *token)
{
	return (token && token->type == T_PIPE);
}

int	check_pipe_errors(t_token *tokens)
{
	t_token	*curr;

	if (!tokens)
		return (0);
	curr = tokens;

	if (is_pipe(curr))
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
	if (is_pipe(curr))
	{
		printf("Syntax error: unexpected token `|'\n");
		return (0);
	}
	return (1);
}

int	check_pipe_redirection(t_token *tokens)
{
	t_token	*curr = tokens;

	while (curr)
	{
		if (curr->type == T_PIPE && curr->next)
		{
			t_token *next = curr->next;
			// Skip les espaces ou T_WORD vides (genre $UNDEFINED_VAR)
			while (next && next->type == T_WORD && (!next->value || !*next->value))
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






int	is_valid_redirection(t_token *token)
{
	if (!token->next || token->next->type != T_WORD)
	{
		printf("Syntax error: expected filename after redirection\n");
		return (0);
	}
	// Interdit d’avoir une redirection vers une autre redirection
	if (token->next->type >= T_REDIR_IN && token->next->type <= T_HEREDOC)
	{
		printf("Syntax error: unexpected token after redirection\n");
		return (0);
	}
	return (1);
}

int	syntax_is_valid(t_token *tokens)
{
	if (!tokens)
		return (0);

	if (!check_pipe_errors(tokens))
		return (0);

	if (!check_pipe_redirection(tokens))
		return (0);

	t_token *curr = tokens;
	while (curr)
	{
		// Redirections sans fichier
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
