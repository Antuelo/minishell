/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:38:10 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/25 14:41:01 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	append_char(char *buffer, int *j, char c)
{
	buffer[*j] = c;
	(*j)++;
}

static void    consume_single_quoted(char *line, char *buffer, t_expand_flag *ctx)
{
    int    start;

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

static void    consume_double_quoted(char *line, char *buffer, t_expand_flag *ctx)
{
    int    start;

    ctx->dq_flag = 1;
    ctx->i++;
    start = ctx->i;
    while (line[ctx->i] && line[ctx->i] != '"')
        ctx->i++;
    while (start < ctx->i)
        append_char(buffer, &ctx->j, line[start++]);
    if (line[ctx->i] == '"')
        ctx->i++;
}

t_token	*handle_combined_word(char *line, int *i)
{
	char			buffer[4096];
	t_expand_flag	ctx;
	t_token			*token;

	ctx.i = *i;
	ctx.j = 0;
	ctx.sq_flag = 0;
	ctx.dq_flag = 0;
	while (line[ctx.i] && !isspace(line[ctx.i]) && !is_operator(line[ctx.i]))
	{
		if (line[ctx.i] == '\'')
			consume_single_quoted(line, buffer, &ctx);
		else if (line[ctx.i] == '"')
			consume_double_quoted(line, buffer, &ctx);
		else
			append_char(buffer, &ctx.j, line[ctx.i++]);
	}
	buffer[ctx.j] = '\0';
	*i = ctx.i;
	token = new_token(buffer, T_WORD);
	if (!token)
		return (NULL);
	token->in_single_quote = ctx.sq_flag;
	token->in_double_quote = ctx.dq_flag;
	return (token);
}
