/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:50:12 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/08 20:51:00 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	append_char(char *buffer, int *j, char c)
{
	buffer[*j] = c;
	(*j)++;
}

static void	consume_single_quoted(char *line, char *buffer, t_expand_flag *ctx)
{
	ctx->sq_flag = 1;
	append_char(buffer, &ctx->j, line[ctx->i++]);
	while (line[ctx->i] && line[ctx->i] != '\'')
		append_char(buffer, &ctx->j, line[ctx->i++]);
	if (line[ctx->i] == '\'')
		append_char(buffer, &ctx->j, line[ctx->i++]);
}

static void	consume_double_quoted(char *line, char *buffer, t_expand_flag *ctx)
{
	ctx->dq_flag = 1;
	ctx->i++;
	while (line[ctx->i] && line[ctx->i] != '"')
		append_char(buffer, &ctx->j, line[ctx->i++]);
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
