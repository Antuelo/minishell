/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:36:59 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/28 22:40:18 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

// Détecte si un caractère est un opérateur spécial du shell
static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

// Associe une string à son type de token
t_token_type	get_token_type(char *str)
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

	// Opérateurs interdits
	if (!strcmp(str, "<<<") || !strcmp(str, ">>>")
		|| !strcmp(str, "||") || !strcmp(str, "|||")
		|| !strcmp(str, "&&") || !strcmp(str, "&&&"))
		return (T_INVALID);

	return (T_WORD); // tout le reste
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
static int	is_invalid_operator_combo(char c1, char c2, char c3)
{
	// Cas de triple opérateur (ex: <<<, >>>, |||, &&&)
	if (c1 == c2 && c2 == c3)
		return (1);
	// Cas de séquences interdites (ex: |<, <|, >|, |>, etc)
	if ((c1 == '|' && (c2 == '<' || c2 == '>')) ||
		((c1 == '<' || c1 == '>') && c2 == '|'))
		return (1);
	return (0);
}

char	*extract_operator(char *line, int *i)
{
	int		start;
	int		len = 1;
	char	c1, c2 = '\0', c3 = '\0';

	start = *i;
	c1 = line[*i];

	if (line[*i + 1])
		c2 = line[*i + 1];
	if (line[*i + 2])
		c3 = line[*i + 2];

	if (is_invalid_operator_combo(c1, c2, c3))
	{
		printf("Syntax error: invalid operator sequence\n");
		return (NULL);
	}

	if ((c1 == '<' || c1 == '>') && c2 == c1)
	{
		len = 2;
		*i += 2;
	}
	else
		*i += 1;

	return (ft_substr(line, start, len));
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
	t_token		*head = NULL;
	t_token		*last = NULL;
	t_token		*new = NULL;
	char		*word;
	int			i = 0;
	char		quote;

	while (line[i])
	{
		while (isspace(line[i]))
			i++;
		if (!line[i])
			break;
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i];
			word = extract_quoted(line, &i, quote);
			if (!word)
				return (free_tokens(head), NULL);
			new = new_token(word, T_WORD);
			free(word);
			if (!new)
				return (free_tokens(head), NULL);
			if (quote == '\'')
				new->in_single_quote = 1;
			else
				new->in_double_quote = 1;
		}
		else if (is_operator(line[i]))
		{
		// Si c’est le dernier caractère, on ne peut pas extraire un opérateur double
			if (!line[i + 1])
			{
				word = strndup(&line[i], 1);
				i++;
			}
			else
				word = extract_operator(line, &i);

			if (!word)
				return (free_tokens(head), NULL);

			new = new_token(word, get_token_type(word));
			free(word);
			if (!new || new->type == T_INVALID)
			{
				if (new && new->type == T_INVALID)
					free(new);
				printf("Syntax error: invalid operator\n");
				return (free_tokens(head), NULL);
			}
		}

		else
		{
			word = extract_word(line, &i);
			if (!word)
				return (free_tokens(head), NULL);
			new = new_token(word, T_WORD);
			free(word);
			if (!new)
				return (free_tokens(head), NULL);
		}
		new->prev = last;
		if (last)
			last->next = new;
		else
			head = new;
		last = new;
	}
	return (head);
}




