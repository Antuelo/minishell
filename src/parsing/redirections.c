/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:45:44 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/15 18:45:44 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"

static int	set_infile(t_cmd *cmd, t_token *token)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = strdup(token->value);
	cmd->append = -1;
	return (1);
}

static int	set_outfile(t_cmd *cmd, t_token *token, int append)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = strdup(token->value);
	cmd->append = append;
	return (1);
}

static int	set_heredoc(t_cmd *cmd, t_token *token)
{
	cmd->heredoc = 1;
	cmd->delimiter = strdup(token->value);
	return (1);
}

int	handle_redirection(t_cmd *cmd, t_token *curr)
{
	t_token	*next;

	next = curr->next;
	if (!next || next->type != T_WORD)
	{
		printf("Syntax error: expected filename after redirection\n");
		return (0);
	}
	if (curr->type == T_REDIR_IN)
		return (set_infile(cmd, next));
	if (curr->type == T_REDIR_OUT)
		return (set_outfile(cmd, next, 0));
	if (curr->type == T_REDIR_APPEND)
		return (set_outfile(cmd, next, 1));
	if (curr->type == T_HEREDOC)
		return (set_heredoc(cmd, next));
	return (1);
}
