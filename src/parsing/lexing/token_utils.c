/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:12:31 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 21:12:35 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Alloue et initialise un nouveau token avec sa valeur et son type
t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->in_single_quote = 0;
	token->in_double_quote = 0;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

// Libère toute la liste chaînée de tokens
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

// Détecte si un caractère est un opérateur spécial du shell
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

// Libère un token individuel (valeur et structure)
void	free_single_token(t_token *token)
{
	if (!token)
		return ;
	free(token->value);
	free(token);
}
