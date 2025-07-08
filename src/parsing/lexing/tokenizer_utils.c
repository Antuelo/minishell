/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:54:24 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 14:54:34 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	append_char(char *buffer, int *j, char c)
{
	buffer[*j] = c;
	(*j)++;
}

static void	consume_single_quoted(char *line, int *i, char *buffer, int *j, int *sq_flag)
{
	(*sq_flag) = 1;
	(*i)++;
	while (line[*i] && line[*i] != '\'')
		append_char(buffer, j, line[(*i)++]);
	if (line[*i] == '\'')
		(*i)++;
}

static void	consume_double_quoted(char *line, int *i, char *buffer, int *j, int *dq_flag)
{
	(*dq_flag) = 1;
	(*i)++;
	while (line[*i] && line[*i] != '"')
		append_char(buffer, j, line[(*i)++]);
	if (line[*i] == '"')
		(*i)++;
}

static void	consume_unquoted(char *line, int *i, char *buffer, int *j)
{
	while (line[*i] && !isspace(line[*i]) && !is_operator(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
		append_char(buffer, j, line[(*i)++]);
}

t_token	*handle_combined_word(char *line, int *i)
{
	char		buffer[4096];
	int			j;
	int			sq;
	int			dq;
	t_token		*token;

	j = 0;
	sq = 0;
	dq = 0;
	while (line[*i] && !isspace(line[*i]) && !is_operator(line[*i]))
	{
		if (line[*i] == '\'')
			consume_single_quoted(line, i, buffer, &j, &sq);
		else if (line[*i] == '"')
			consume_double_quoted(line, i, buffer, &j, &dq);
		else
			consume_unquoted(line, i, buffer, &j);
	}
	buffer[j] = '\0';
	token = new_token(buffer, T_WORD);
	if (!token)
		return (NULL);
	token->in_single_quote = sq;
	token->in_double_quote = dq;
	return (token);
}
