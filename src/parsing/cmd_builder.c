/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 27/05/2025 19:47:25 by llabatut          #+#    #+#             */
/*   Updated: 28/05/2025 18:40:52 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static t_token	*next_pipe(t_token *token)
{
	while (token && token->type != T_PIPE)
		token = token->next;
	return (token);
}

t_cmd *init_cmd(void)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delimiter = NULL;
	cmd->args = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}


t_cmd	*build_cmd_list_from_tokens(t_token *tokens)
{
	t_cmd	*head = NULL;
	t_cmd	*last = NULL;
	t_cmd	*new;
	t_token	*start = tokens;
	t_token	*pipe;
	t_token	*cut;

	while (start)
	{
		pipe = next_pipe(start);
		if (pipe)
		{
			cut = pipe->next;
			if (pipe->prev)
				pipe->prev->next = NULL;
			pipe->prev = NULL;
			pipe->next = NULL;
		}
		else
			cut = NULL;

		new = init_cmd();
		if (!new || !fill_cmd_from_tokens(start, new))
		{
			if (new)
				free_cmd(new);
			free_cmd_list(head);
			return (NULL);
		}
		if (last)
		{
			last->next = new;
			new->prev = last;
		}
		else
			head = new;
		last = new;
		start = cut;
	}
	return (head);
}
