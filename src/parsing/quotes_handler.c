/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 24/05/2025 16:20:33 by llabatut          #+#    #+#             */
/*   Updated: 24/05/2025 16:20:55 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/parsing.h"

// Vérifie s'il y a des quotes non fermées dans la ligne
int	check_unclosed_quotes(const char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i++];
			while (line[i])
			{
				if (line[i] == quote)
					break;
				if (line[i] == '\\' && quote == '"')
					i++; // saute le caractère échappé
				i++;
			}
			if (!line[i] || line[i] != quote)
				return (1); // quote non fermée
			i++; // skip la quote fermée
		}
		else
			i++;
	}
	return (0);
}


// Supprime les quotes dans une string
static char	*remove_quotes(char *str)
{
	char	*res;
	int		i;
	int		j;
	char	quote;

	res = malloc(strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				res[j++] = str[i++];
			if (str[i])
				i++;
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

// Supprime les quotes dans tous les T_WORD après expansion
void	remove_quotes_from_tokens(t_token *tokens)
{
	char	*clean;

	while (tokens)
	{
		if (tokens->type == T_WORD && tokens->value
			&& (strchr(tokens->value, '"') || strchr(tokens->value, '\'')))
		{
			clean = remove_quotes(tokens->value);
			free(tokens->value);
			tokens->value = clean;
		}
		tokens = tokens->next;
	}
}
