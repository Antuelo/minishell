/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:11:37 by llabatut          #+#    #+#             */
/*   Updated: 2025/08/21 20:11:56 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/* ---------- helpers (≤4 args) ---------- */

static int	collect_sq(char *line, int *i, char *buf, int *j)
{
	int	had;

	had = 1;
	(*i)++;
	while (line[*i] && line[*i] != '\'')
	{
		buf[*j] = line[*i];
		(*j)++;
		(*i)++;
	}
	if (line[*i] == '\'')
		(*i)++;
	return (had);
}

static int	collect_dq(char *line, int *i, char *buf, int *j)
{
	int	had;

	had = 1;
	(*i)++;
	while (line[*i] && line[*i] != '"')
	{
		buf[*j] = line[*i];
		(*j)++;
		(*i)++;
	}
	if (line[*i] == '"')
		(*i)++;
	return (had);
}

static void	collect_pln(char *line, int *i, char *buf, int *j)
{
	buf[*j] = line[*i];
	(*j)++;
	(*i)++;
}

static void	process_loop(t_hw *c)
{
	while (c->line[*c->i]
		&& !isspace(c->line[*c->i])
		&& !is_operator(c->line[*c->i]))
	{
		if (c->line[*c->i] == '\'')
		{
			if (collect_sq(c->line, c->i, c->buf, &c->j))
				c->sq = 1;
		}
		else if (c->line[*c->i] == '"')
		{
			if (collect_dq(c->line, c->i, c->buf, &c->j))
				c->dq = 1;
		}
		else
			collect_pln(c->line, c->i, c->buf, &c->j);
	}
}

t_token	*handle_combined_word(char *line, int *i)
{
	char	buffer[4096];
	t_hw	c;
	t_token	*tok;

	c.line = line;
	c.i = i;
	c.buf = buffer;
	c.j = 0;
	c.sq = 0;
	c.dq = 0;
	process_loop(&c);
	buffer[c.j] = '\0';
	tok = new_token(buffer, T_WORD);
	if (!tok)
		return (NULL);
	tok->in_single_quote = c.sq;
	tok->in_double_quote = c.dq;
	return (tok);
}
