/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:43:55 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/28 17:53:24 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

/*static void	append_char(char *buffer, int *j, char c)
{
	buffer[*j] = c;
	(*j)++;
}

static void	consume_single_quoted(char *line, char *buffer, t_expand_flag *ctx)
{
	int	start;

	ctx->sq_flag = 1;
	ctx->i++;
	start = ctx->i;
	while (line[ctx->i] && line[ctx->i] != '\'')
		ctx->i++;
	while (start < ctx->i)
		append_char(buffer, &ctx->j, line[start++]);
	if (line[ctx->i] == '\'')
		ctx->i++;
}

static void	consume_double_quoted(char *line, char *buffer, t_expand_flag *ctx)
{
	int	start;

	ctx->dq_flag = 1;
	ctx->i++;
	start = ctx->i;
	while (line[ctx->i] && line[ctx->i] != '"')
		ctx->i++;
	while (start < ctx->i)
		append_char(buffer, &ctx->j, line[start++]);
	if (line[ctx->i] == '"')
		ctx->i++;
}*/

t_token	*handle_combined_word(char *line, int *i)
{
	char		buffer[4096];
	int			j = 0;
	int			sq_flag = 0;
	int			dq_flag = 0;

	while (line[*i] && !isspace(line[*i]) && !is_operator(line[*i]))
	{
		if (line[*i] == '\'')
		{
			sq_flag = 1;
			(*i)++;
			while (line[*i] && line[*i] != '\'')
				buffer[j++] = line[(*i)++];
			if (line[*i] == '\'')
				(*i)++;
		}
		else if (line[*i] == '"')
		{
			dq_flag = 1;
			(*i)++;
			while (line[*i] && line[*i] != '"')
				buffer[j++] = line[(*i)++];
			if (line[*i] == '"')
				(*i)++;
		}
		else
			buffer[j++] = line[(*i)++];
	}
	buffer[j] = '\0';

	t_token *token = new_token(buffer, T_WORD);
	if (!token)
		return (NULL);
	token->in_single_quote = sq_flag;
	token->in_double_quote = dq_flag;
	return (token);
}

