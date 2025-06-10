/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:07:07 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/10 18:07:07 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Gère les caractères échappés dans les quotes doubles (ex: \" ou \\)
static void	handle_double_quote_escape(char *line, int *i, char *res, int *j)
{
	if (line[*i + 1] == '"' || line[*i + 1] == '\\')
	{
		(*i)++;
		res[*j] = line[*i];
		(*j)++;
		(*i)++;
	}
	else
	{
		res[*j] = line[*i];
		(*j)++;
		(*i)++;
	}
}

// Extrait le contenu d’une chaîne entre quotes, en gérant les échappements
static char	*extract_quoted(char *line, int *i, char quote)
{
	char	*res;
	int		j;

	res = malloc(strlen(line) + 1);
	if (!res)
		return (NULL);
	j = 0;
	(*i)++;
	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '"')
			handle_double_quote_escape(line, i, res, &j);
		else
		{
			res[j] = line[*i];
			j++;
			(*i)++;
		}
	}
	if (line[*i] == quote)
		(*i)++;
	res[j] = '\0';
	return (res);
}

// Crée un token de type WORD à partir d’une chaîne entre quotes
t_token	*handle_quoted_token(char *line, int *i)
{
	char	*word;
	t_token	*new;
	char	quote;

	quote = line[*i];
	word = extract_quoted(line, i, quote);
	if (!word)
		return (NULL);
	new = new_token(word, T_WORD);
	free(word);
	if (!new)
		return (NULL);
	if (quote == '\'')
		new->in_single_quote = 1;
	else
		new->in_double_quote = 1;
	return (new);
}
