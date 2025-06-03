/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:42:59 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 20:42:59 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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
	if (!strcmp(str, "<<<") || !strcmp(str, ">>>")
		|| !strcmp(str, "||") || !strcmp(str, "|||")
		|| !strcmp(str, "&&") || !strcmp(str, "&&&"))
		return (T_INVALID);
	return (T_WORD);
}

// Extrait un opérateur (<, <<, >, >>, |) et détecte les erreurs type >>>>
static int	is_invalid_operator_combo(char c1, char c2, char c3)
{
	if (c1 == c2 && c2 == c3)
		return (1);
	if ((c1 == '|' && (c2 == '<' || c2 == '>'))
		|| ((c1 == '<' || c1 == '>') && c2 == '|'))
		return (1);
	return (0);
}

static int	get_operator_length(char c1, char c2, char c3, int *i)
{
	if (is_invalid_operator_combo(c1, c2, c3))
	{
		printf("Syntax error: invalid operator sequence\n");
		return (-1);
	}
	if ((c1 == '<' && c2 == '<') || (c1 == '>' && c2 == '>')
		|| (c1 == '|' && c2 == '|') || (c1 == '&' && c2 == '&'))
	{
		*i += 2;
		return (2);
	}
	*i += 1;
	return (1);
}

static char	*extract_operator(char *line, int *i)
{
	int		start;
	int		len;
	char	c1;
	char	c2;
	char	c3;

	start = *i;
	c1 = line[*i];
	c2 = line[*i + 1];
	c3 = line[*i + 2];
	len = get_operator_length(c1, c2, c3, i);
	if (len == -1)
		return (NULL);
	return (ft_substr(line, start, len));
}

t_token	*handle_operator_token(char *line, int *i)
{
	char	*word;
	t_token	*new;

	if (!line[*i + 1])
	{
		word = strndup(&line[*i], 1);
		(*i)++;
	}
	else
		word = extract_operator(line, i);
	if (!word)
		return (NULL);
	new = new_token(word, get_token_type(word));
	free(word);
	if (!new || new->type == T_INVALID)
	{
		free_single_token(new);
		printf("Syntax error: invalid operator\n");
		return (NULL);
	}
	return (new);
}
