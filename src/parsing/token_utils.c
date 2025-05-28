/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:53:51 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/28 21:56:32 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

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
	token->next = NULL;
	token->prev = NULL;
	token->in_single_quote = 0;
	token->in_double_quote = 0;
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


// Affiche tous les tokens pour le debug (valeur + type numérique)
void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: [%s] | Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}
