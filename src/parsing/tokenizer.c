/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:15:39 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/12 18:15:39 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

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

	start = *i;
	count = 1;
	if ((line[*i] == '<' || line[*i] == '>') && line[*i + 1] == line[*i])
	{
		count++;
		(*i)++;
	}
	(*i)++;
	if (line[*i] == line[start])
		return (NULL);
	return (strndup(line + start, count));
}


static char	*extract_word(char *line, int *i)
{
	int		start;

	start = *i;
	while (line[*i] && !isspace(line[*i]) && !is_operator(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
			break ;
		if (line[*i] == '\\')
			(*i)++;
		(*i)++;
	}
	return (strndup(line + start, *i - start));
}

t_token	*tokenize(char *line)
{
	t_token	*head = NULL;
	t_token	**curr = &head;
	char	*word;
	int		i = 0;

	while (line[i])
	{
		while (isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '\'' || line[i] == '"')
			word = extract_quoted(line, &i);
		else if (is_operator(line[i]))
        {
	        word = extract_operator(line, &i);
	        if (!word)
	        {
		        printf("Syntax error : invalid redirection\n");
		        free_tokens(head);
		        return (NULL);
	        }
        }
		else
			word = extract_word(line, &i);
		if (!word)
			return (free_tokens(head), NULL);
		*curr = new_token(word, get_token_type(word));
		if (!*curr)
			return (free(word), free_tokens(head), NULL);
		curr = &(*curr)->next;
	}
	return (head);
}
