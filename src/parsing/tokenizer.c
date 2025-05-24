/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 16:16:57 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/24 16:19:37 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Détecte si un caractère est un opérateur spécial du shell
static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// Associe une string à son type de token
static t_token_type	get_token_type(char *str)
{
	if (!strcmp(str, "|"))
		return (T_PIPE);
	if (!strcmp(str, "<"))
		return (T_REDIR_IN);
	if (!strcmp(str, ">"))
		return (T_REDIR_OUT);
	if (!strcmp(str, "<<"))
		return (T_HEREDOC);
	if (!strcmp(str, ">>"))
		return (T_REDIR_APPEND);
	return (T_WORD);
}

// Extrait une chaîne entre quotes (simples ou doubles)
static char	*extract_quoted(char *line, int *i, char quote)
{
	char	*res;
	int		j;

	res = malloc(strlen(line) + 1);
	if (!res)
		return (NULL);
	j = 0;
	(*i)++; // Skip opening quote

	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '"')
		{
			if (line[*i + 1] == '"' || line[*i + 1] == '\\')
			{
				(*i)++; // skip backslash
				res[j++] = line[(*i)++]; // copy next char
			}
			else
			{
				res[j++] = line[(*i)++];
			}
		}
		else
		{
			res[j++] = line[(*i)++];
		}
	}
	if (line[*i] == quote)
		(*i)++; // Skip closing quote

	res[j] = '\0';
	return (res);
}

// Extrait un opérateur (<, <<, >, >>, |) et détecte les erreurs type >>>>
static char	*extract_operator(char *line, int *i)
{
	int		start;
	int		count;
	char	op;

	start = *i;
	op = line[*i];
	count = 1;
	(*i)++;
	if (line[*i] == op)
	{
		count++;
		(*i)++;
	}
	// Si un troisième caractère identique est présent (>>> ou <<<), on considère que c'est invalide
	if (line[*i] == op)
		return (NULL);
	return (strndup(line + start, count));
}

// Extrait un mot (commande, argument) jusqu’à un espace ou un caractère spécial
static char	*extract_word(char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i]
		&& !isspace(line[*i])
		&& !is_operator(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
	{
		// On saute le caractère suivant s’il est échappé par '\'
		if (line[*i] == '\\')
			(*i)++;
		(*i)++;
	}
	return (strndup(line + start, *i - start));
}

// Découpe la ligne en tokens liés entre eux (liste chaînée)
t_token	*tokenize(char *line)
{
	t_token		*head; // Premier token
	t_token		*last; // Dernier token ajouté
	t_token		*new;  // Token temporaire
	char		*word;
	int			i;

	head = NULL;
	last = NULL;
	i = 0;

	while (line[i])
	{
		while (isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		// Extraction selon le type de caractère rencontré
		if (line[i] == '\'' || line[i] == '"')
		{
			char quote = line[i];
			word = extract_quoted(line, &i, quote);
			new = new_token(word, T_WORD);
			if (quote == '\'')
				new->in_single_quote = 1;
			else
				new->in_double_quote = 1;
		}
		else if (is_operator(line[i]))
		{
			word = extract_operator(line, &i);
			if (!word)
				return (free_tokens(head), printf("Syntax error\n"), NULL);
			new = new_token(word, get_token_type(word));
		}
		else
		{
			word = extract_word(line, &i);
			new = new_token(word, T_WORD);
		}
		if (!new)
			return (free(word), free_tokens(head), NULL);
		new->prev = last;
		if (last)
			last->next = new;
		else
			head = new;
		last = new;
	}
	return (head);
}

