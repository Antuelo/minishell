/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:27:19 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/19 19:27:19 by llabatut         ###   ########.ch       */
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
static char	*extract_quoted(char *line, int *i)
{
	char	quote;
	int		start;

	quote = line[*i];
	start = ++(*i);
	while (line[*i] && line[*i] != quote)
	{
		// On gère les backslashes uniquement dans les double quotes
		if (line[*i] == '\\' && quote == '"')
			(*i)++;
		(*i)++;
	}
	// strndup extrait la partie entre les quotes
	return (strndup(line + start, (*i)++ - start));
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
		// Ignore les espaces
		while (isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		// Extraction selon le type de caractère rencontré
		if (line[i] == '\'' || line[i] == '"')
			word = extract_quoted(line, &i);
		else if (is_operator(line[i]))
			word = extract_operator(line, &i);
		else
			word = extract_word(line, &i);
		if (!word)
			return (free_tokens(head), printf("Syntax error\n"), NULL);

		// Crée le token avec son type, et le relie à la liste
		new = new_token(word, get_token_type(word));
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
