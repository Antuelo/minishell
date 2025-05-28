/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:43:48 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/28 22:43:48 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

t_token	*next_pipe(t_token *token)
{
	while (token && token->type != T_PIPE)
		token = token->next;
	return (token);
}

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delimiter = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;
	cmd->hdoc_pipe[0] = -1;
	cmd->hdoc_pipe[1] = -1;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}


t_token	*last_token(t_token *start)
{
	t_token	*tmp = start;

	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

t_cmd	*build_cmd_list_from_tokens(t_token *tokens)
{
	t_cmd	*head = NULL;
	t_cmd	*last = NULL;
	t_cmd	*new;
	t_token	*start = tokens;
	t_token	*pipe;

	while (start)
	{
		pipe = next_pipe(start);
		t_token *next_cmd = NULL;

		if (pipe)
		{
			next_cmd = pipe->next;
			pipe->prev->next = NULL;  // couper temporairement
			pipe->prev = NULL;
		}

		new = init_cmd();
		if (!new)
		{
			free_cmd_list(head);
			return (NULL);
		}

		if (!fill_cmd_from_tokens(start, new))
		{
			if (pipe)
			{
				// Restaure la liaison pour permettre free_tokens
				start = pipe;
				if (pipe->next)
					pipe->next->prev = pipe;
			}
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

		// Restore link après parsing
		if (pipe)
		{
			pipe->prev = last_token(start);
			if (pipe->prev)
				pipe->prev->next = pipe;
		}
		start = next_cmd;
	}
	return (head);
}
