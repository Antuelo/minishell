/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:15:23 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/12 18:15:23 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"


t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: [%s] | Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}
