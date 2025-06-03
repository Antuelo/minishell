/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:42:38 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 20:42:38 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
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

t_cmd	*build_cmd_list_from_tokens(t_token *tokens)
{
	t_cmd		*head;
	t_cmd		*last;
	t_token		*curr;
	t_token		*start;
	int			ok;

	head = NULL;
	last = NULL;
	start = tokens;
	curr = tokens;
	while (curr)
	{
		ok = 1;
		if (curr->type == T_PIPE || curr->next == NULL)
			ok = process_command(&head, &last, &start, curr);
		if (!ok)
			return (free_cmd_list(head), NULL);
		curr = curr->next;
	}
	return (head);
}
