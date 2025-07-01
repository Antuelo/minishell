/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:09:13 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/01 19:09:17 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

// Extrait un mot (commande, argument) jusqu’à un espace ou un caractère spécial
static char	*extract_word(char *line, int *i)
{
	char	*word;
	int		j;
	int		len;

	len = strlen(line + *i);
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (line[*i]
		&& !isspace(line[*i])
		&& !is_operator(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
	{
		if (line[*i] == '\\' && line[*i + 1])
		{
			(*i)++;
			word[j++] = line[*i];
		}
		else
			word[j++] = line[*i];
		(*i)++;
	}
	word[j] = '\0';
	return (word);
}

// Crée un token de type T_WORD à partir d’un mot extrait de la ligne
t_token	*handle_word_token(char *line, int *i)
{
	char	*word;
	t_token	*new;

	word = extract_word(line, i);
	if (!word)
	{
		return (NULL);
	}
	new = new_token(word, T_WORD);
	free(word);
	if (!new)
	{
		return (NULL);
	}
	return (new);
}
