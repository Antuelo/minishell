/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:45:56 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/15 18:46:26 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

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

static char	*extract_quoted(char *line, int *i)
{
	char	quote;
	int		start;

	quote = line[*i];
	start = ++(*i);
	while (line[*i] && line[*i] != quote)
	{
		if (line[*i] == '\\' && quote == '"')
			(*i)++;
		(*i)++;
	}
	return (strndup(line + start, (*i)++ - start));
}

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
	if (line[*i] == op)
		return (NULL);
	return (strndup(line + start, count));
}

static char	*extract_word(char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i]
		&& !isspace(line[*i])
		&& !is_operator(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
	{
		if (line[*i] == '\\')
			(*i)++;
		(*i)++;
	}
	return (strndup(line + start, *i - start));
}

t_token	*tokenize(char *line)
{
	t_token		*head;
	t_token		*last;
	t_token		*new;
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
		if (line[i] == '\'' || line[i] == '"')
			word = extract_quoted(line, &i);
		else if (is_operator(line[i]))
			word = extract_operator(line, &i);
		else
			word = extract_word(line, &i);
		if (!word)
			return (free_tokens(head), printf("Syntax error\n"), NULL);
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