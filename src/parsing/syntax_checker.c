/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:32:39 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 19:32:39 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
