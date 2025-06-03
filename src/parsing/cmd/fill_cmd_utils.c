/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 19:30:18 by llabatut          #+#    #+#             */
/*   Updated: 2025/06/03 19:30:18 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// Compte le nombre d'arguments valides
int	count_args(t_token *tokens)
{
	t_token	*curr;
	int		count;

	count = 0;
	curr = tokens;
	while (curr)
	{
		if (curr->type == T_WORD)
		{
			if (curr->prev
				&& (curr->prev->type == T_REDIR_IN
					|| curr->prev->type == T_REDIR_OUT
					|| curr->prev->type == T_REDIR_APPEND
					|| curr->prev->type == T_HEREDOC))
			{
				curr = curr->next;
				continue ;
			}
			count++;
		}
		curr = curr->next;
	}
	return (count);
}

int	allocate_args_array(t_token *tokens, t_cmd *cmd)
{
	int	arg_count;
	int	i;

	arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (0);
	i = 0;
	while (i <= arg_count)
		cmd->args[i++] = NULL;
	return (1);
}

void	init_cmd_fields(t_cmd *cmd)
{
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delimiter = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;
}

int	copy_argument(t_cmd *cmd, char *value, int *i)
{
	cmd->args[*i] = strdup(value);
	if (!cmd->args[*i])
	{
		while (*i > 0)
		{
			(*i)--;
			free(cmd->args[*i]);
		}
		free(cmd->args);
		cmd->args = NULL;
		return (0);
	}
	(*i)++;
	return (1);
}

int	handle_redir_fail(t_cmd *cmd, t_token *curr, int i)
{
	if (!handle_redirection(cmd, curr))
	{
		if (cmd->delimiter)
		{
			free(cmd->delimiter);
			cmd->delimiter = NULL;
		}
		while (i > 0)
		{
			i--;
			free(cmd->args[i]);
		}
		free(cmd->args);
		return (0);
	}
	return (1);
}
