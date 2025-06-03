/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:31:36 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 19:31:36 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*process_token_segment(char *line, int *i)
{
	t_token	*token;

	if (is_operator(line[*i]))
		token = handle_operator_token(line, i);
	else if (line[*i] == '\'' || line[*i] == '\"')
		token = handle_quoted_token(line, i);
	else
		token = handle_word_token(line, i);
	if (!token)
	{
		return (NULL);
	}
	if (!token->value || !*token->value)
	{
		free_single_token(token);
		return (NULL);
	}
	return (token);
}

void	append_token(t_token **head, t_token **last, t_token *new)
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
