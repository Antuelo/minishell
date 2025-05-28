/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llabatut <llabatut@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:32:20 by llabatut          #+#    #+#             */
/*   Updated: 2025/05/28 22:35:09 by llabatut         ###   ########.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parsing.h"



int	fill_cmd_from_tokens(t_token *tokens, t_cmd *cmd)
{
	t_token	*curr;
	int		arg_count;
	int		i;

	arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (0);
	i = 0;
	while (i <= arg_count)
	{
		cmd->args[i] = NULL;
		i++;
	}

	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->delimiter = NULL;
	cmd->append = -1;
	cmd->heredoc = 0;

	curr = tokens;
	i = 0;
	while (curr)
	{
		if (curr->type == T_WORD)
		{
			if (curr->prev && curr->prev->type >= T_REDIR_IN
				&& curr->prev->type <= T_HEREDOC)
			{
				curr = curr->next;
				continue ;
			}
			cmd->args[i] = strdup(curr->value);
			if (!cmd->args[i])
			{
				while (i > 0)
				{
					i--;
					free(cmd->args[i]);
				}
				free(cmd->args);
				return (0);
			}
			i++;
		}
		else if (curr->type >= T_REDIR_IN && curr->type <= T_HEREDOC)
		{
			if (!handle_redirection(cmd, curr))
			{
				while (i > 0)
				{
					i--;
					free(cmd->args[i]);
				}
				free(cmd->args);
				return (0);
			}
			curr = curr->next;
		}
		if (curr)
			curr = curr->next;
	}
	cmd->args[i] = NULL;
	return (1);
}





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
			// Si ce T_WORD est lié à une redirection (ex: fichier après >), on le compte pas
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
