/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:25:08 by llabatut          #+#    #+#             */
/*   Updated: 2025/07/28 18:25:34 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static void	init_buffer_flags(char *buffer, int *j, int *sq_flag, int *dq_flag)
{
	int	i;

	i = 0;
	while (i < 4096)
		buffer[i++] = '\0';
	*j = 0;
	*sq_flag = 0;
	*dq_flag = 0;
}

static void	handle_single_quote(char *line, int *i, char *buffer, int *j)
{
	(*i)++;
	while (line[*i] && line[*i] != '\'')
		buffer[(*j)++] = line[(*i)++];
	if (line[*i] == '\'')
		(*i)++;
}

static void	handle_double_quote(char *line, int *i, char *buffer, int *j)
{
	(*i)++;
	while (line[*i] && line[*i] != '"')
		buffer[(*j)++] = line[(*i)++];
	if (line[*i] == '"')
		(*i)++;
}

static void	fill_buffer_loop(t_bufdata *data)
{
	while (data->line[*data->i] && !isspace(data->line[*data->i])
		&& !is_operator(data->line[*data->i]))
	{
		if (data->line[*data->i] == '\'')
		{
			*data->sq_flag = 1;
			handle_single_quote(data->line, data->i, data->buffer, data->j);
		}
		else if (data->line[*data->i] == '"')
		{
			*data->dq_flag = 1;
			handle_double_quote(data->line, data->i, data->buffer, data->j);
		}
		else
			data->buffer[(*data->j)++] = data->line[(*data->i)++];
	}
	data->buffer[*data->j] = '\0';
}

t_token	*handle_combined_word(char *line, int *i)
{
	t_wordinfo	info;
	t_token		*token;
	t_bufdata	data;

	init_buffer_flags(info.buffer, &info.j, &info.sq_flag, &info.dq_flag);
	data.line = line;
	data.i = i;
	data.buffer = info.buffer;
	data.j = &info.j;
	data.sq_flag = &info.sq_flag;
	data.dq_flag = &info.dq_flag;
	fill_buffer_loop(&data);
	token = new_token(info.buffer, T_WORD);
	if (!token)
		return (NULL);
	token->in_single_quote = info.sq_flag;
	token->in_double_quote = info.dq_flag;
	return (token);
}
