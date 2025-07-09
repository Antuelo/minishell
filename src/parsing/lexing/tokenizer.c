/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:55:20 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 14:55:25 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Traite un segment de la ligne et retourne le token correspondant
static t_token	*process_token_segment(char *line, int *i)
{
	t_token	*token;

	if (is_operator(line[*i]))
		token = handle_operator_token(line, i);
	else
		token = handle_combined_word(line, i);
	if (!token || !token->value)
	{
		free_single_token(token);
		return (NULL);
	}
	return (token);
}

// Ajoute un nouveau token à la fin de la liste chaînée
static void	append_token(t_token **head, t_token **last, t_token *new)
{
	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		*last = new;
	}
	else
	{
		(*last)->next = new;
		new->prev = *last;
		*last = new;
	}
}

// Transforme la ligne d'entrée en liste chaînée de tokens
t_token	*tokenize(char *line)
{
	t_token	*head;
	t_token	*last;
	t_token	*new;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	while (line[i])
	{
		while (isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		new = process_token_segment(line, &i);
		if (!new || new == (t_token *)-1)
		{
			free_tokens(head);
			return (NULL);
		}
		append_token(&head, &last, new);
	}
	return (head);
}
